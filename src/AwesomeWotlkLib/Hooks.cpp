#include "Hooks.h"
#include <Windows.h>
#include <Detours/detours.h>
#include <string>

static Console::CVar* s_cvar_spellProjectionHorizontalBias;
static Console::CVar* s_cvar_spellProjectionMaxRange;
static Console::CVar* s_cvar_spellProjectionMode;
static int CVarHandler_spellProjectionHorizontalBias(Console::CVar*, const char*, const char* value, LPVOID) { return 1; }
static int CVarHandler_spellProjectionMaxRange(Console::CVar*, const char*, const char* value, LPVOID) { return 1; }
static int CVarHandler_spellProjectionMode(Console::CVar*, const char*, const char* value, LPVOID) { return 1; }

static constexpr float MAX_TRACE_DISTANCE = 1000.0f;
static constexpr uint32_t TERRAIN_HIT_FLAGS = 0x10111;

static bool g_cursorKeywordActive = false;
static bool g_playerLocationKeywordActive = false;
static bool g_hasAdjustedPos = false;
static C3Vector g_adjustedSpellPos = { 0, 0, 0 };

typedef int(__stdcall* ProcessAoETargeting)(uint32_t* a1);
static ProcessAoETargeting ProcessAoETargeting_orig = (ProcessAoETargeting)0x004F66C0;

typedef int(__cdecl* GetSpellRange)(int, int, float*, float*, int);
static GetSpellRange GetSpellRange_orig = (GetSpellRange)0x00802C30;

static float clampf(float val, float minVal, float maxVal) { return (val < minVal) ? minVal : (val > maxVal) ? maxVal : val; }
static float dot(const C3Vector& a, const C3Vector& b) { return a.X * b.X + a.Y * b.Y + a.Z * b.Z; }

typedef int(__cdecl* SecureCmdOptionParse_t)(lua_State* L);
static SecureCmdOptionParse_t SecureCmdOptionParse_orig = (SecureCmdOptionParse_t)0x00564AE0;

typedef void(__cdecl* HandleTerrainClick_t)(TerrainClickEvent*);
static HandleTerrainClick_t HandleTerrainClick_orig = (HandleTerrainClick_t)0x00527830;

typedef uint8_t(__cdecl* TraceLine_t)(C3Vector* start, C3Vector* end, C3Vector* hitPoint, float* distance, uint32_t flag, uint32_t optional);
static TraceLine_t TraceLine_orig = (TraceLine_t)0x007A3B70;

struct CVarArgs {
    Console::CVar** dst;
    const char* name;
    const char* desc;
    Console::CVarFlags flags;
    const char* initialValue;
    Console::CVar::Handler_t func;
};

static std::vector<CVarArgs> s_customCVars;
void Hooks::FrameXML::registerCVar(Console::CVar** dst, const char* str, const char* desc, Console::CVarFlags flags, const char* initialValue, Console::CVar::Handler_t func)
{
    s_customCVars.push_back({ dst, str, desc, flags, initialValue, func });
}

static void(*CVars_Initialize_orig)() = (decltype(CVars_Initialize_orig))0x0051D9B0;
static void CVars_Initialize_hk()
{
    CVars_Initialize_orig();
    for (const auto& [dst, name, desc, flags, initialValue, func] : s_customCVars) {
        Console::CVar* cvar = Console::RegisterCVar(name, desc, flags, initialValue, func, 0, 0, 0, 0);
        if (dst) *dst = cvar;
    }
}


static std::vector<const char*> s_customEvents;
void Hooks::FrameXML::registerEvent(const char* str) { s_customEvents.push_back(str); }

static void (*FrameScript_FillEvents_orig)(const char** list, size_t count) = (decltype(FrameScript_FillEvents_orig))0x0081B5F0;
static void FrameScript_FillEvents_hk(const char** list, size_t count)
{
    std::vector<const char*> events;
    events.reserve(count + s_customEvents.size());
    events.insert(events.end(), &list[0], &list[count]);
    events.insert(events.end(), s_customEvents.begin(), s_customEvents.end());
    FrameScript_FillEvents_orig(events.data(), events.size());
}


static std::vector<lua_CFunction> s_customLuaLibs;
void Hooks::FrameXML::registerLuaLib(lua_CFunction func) { s_customLuaLibs.push_back(func); }

static void Lua_OpenFrameXMlApi_bulk()
{
    lua_State* L = GetLuaState();
    for (auto& func : s_customLuaLibs)
        func(L);
}

static void(*Lua_OpenFrameXMLApi_orig)() = (decltype(Lua_OpenFrameXMLApi_orig))0x00530F85;
static void __declspec(naked) Lua_OpenFrameXMLApi_hk()
{
    __asm {
        pushad;
        pushfd;
        call Lua_OpenFrameXMlApi_bulk;
        popfd;
        popad;
        ret;
    }
}

struct CustomTokenDetails {
    CustomTokenDetails() { memset(this, NULL, sizeof(*this)); }
    CustomTokenDetails(Hooks::FrameScript::TokenGuidGetter* getGuid, Hooks::FrameScript::TokenIdGetter* getId)
        : hasN(false), getGuid(getGuid), getId(getId)
    {
    }
    CustomTokenDetails(Hooks::FrameScript::TokenNGuidGetter* getGuid, Hooks::FrameScript::TokenIdNGetter* getId)
        : hasN(true), getGuidN(getGuid), getIdN(getId)
    {
    }

    bool hasN;
    union {
        Hooks::FrameScript::TokenGuidGetter* getGuid;
        Hooks::FrameScript::TokenNGuidGetter* getGuidN;
    };
    union {
        Hooks::FrameScript::TokenIdGetter* getId;
        Hooks::FrameScript::TokenIdNGetter* getIdN;
    };
};
static std::unordered_map<std::string, CustomTokenDetails> s_customTokens;
void Hooks::FrameScript::registerToken(const char* token, TokenGuidGetter* getGuid, TokenIdGetter* getId) { s_customTokens[token] = { getGuid, getId }; }
void Hooks::FrameScript::registerToken(const char* token, TokenNGuidGetter* getGuid, TokenIdNGetter* getId) { s_customTokens[token] = { getGuid, getId }; }

static DWORD_PTR GetGuidByKeyword_jmpbackaddr = 0;
static void GetGuidByKeyword_bulk(const char** stackStr, guid_t* guid)
{
    for (auto& [token, conv] : s_customTokens) {
        if (strncmp(*stackStr, token.data(), token.size()) == 0) {
            *stackStr += token.size();
            if (conv.hasN) {
                int n = gc_atoi(stackStr);
                *guid = n > 0 ? conv.getGuidN(n - 1) : 0;
            }
            else {
                *guid = conv.getGuid();
            }
            GetGuidByKeyword_jmpbackaddr = 0x0060AD57;
            return;
        }
    }
    GetGuidByKeyword_jmpbackaddr = 0x0060AD44;
}

static void(*GetGuidByKeyword_orig)() = (decltype(GetGuidByKeyword_orig))0x0060AFAA;
static void __declspec(naked) GetGuidByKeyword_hk()
{
    __asm {
        pushad;
        pushfd;
        push[ebp + 0xC];
        lea eax, [ebp + 0x8];
        push eax;
        call GetGuidByKeyword_bulk;
        add esp, 8;
        popfd;
        popad;

        push GetGuidByKeyword_jmpbackaddr;
        ret;
    }
}

static char** (*GetKeywordsByGuid_orig)(guid_t* guid, size_t* size) = (decltype(GetKeywordsByGuid_orig))0x0060BB70;
static char** GetKeywordsByGuid_hk(guid_t* guid, size_t* size)
{
    char** buf = GetKeywordsByGuid_orig(guid, size);
    if (!buf) return buf;
    for (auto& [token, conv] : s_customTokens) {
        if (*size >= 8) break;
        if (conv.hasN) {
            int id = conv.getIdN(*guid);
            if (id >= 0)
                snprintf(buf[(*size)++], 32, "%s%d", token.c_str(), id + 1);
        }
        else {
            if (conv.getId(*guid))
                snprintf(buf[(*size)++], 32, "%s", token.c_str());
        }
    }
    return buf;
}

static std::vector<Hooks::DummyCallback_t> s_customOnUpdate;
void Hooks::FrameScript::registerOnUpdate(DummyCallback_t func) { s_customOnUpdate.push_back(func); }

static int(*FrameScript_FireOnUpdate_orig)(int a1, int a2, int a3, int a4) = (decltype(FrameScript_FireOnUpdate_orig))0x00495810;
static int FrameScript_FireOnUpdate_hk(int a1, int a2, int a3, int a4)
{
    for (auto func : s_customOnUpdate)
        func();
    return FrameScript_FireOnUpdate_orig(a1, a2, a3, a4);
}

static std::vector<Hooks::DummyCallback_t> s_glueXmlPostLoad;
void Hooks::GlueXML::registerPostLoad(DummyCallback_t func) { s_glueXmlPostLoad.push_back(func); }

static void LoadGlueXML_bulk() { for (auto func : s_glueXmlPostLoad) func(); }

static void (*LoadGlueXML_orig)() = (decltype(LoadGlueXML_orig))0x004DA9AC;
static void __declspec(naked) LoadGlueXML_hk()
{
    __asm {
        pop ebx;
        mov esp, ebp;
        pop ebp;

        pushad;
        pushfd;
        call LoadGlueXML_bulk;
        popfd;
        popad;
        ret;
    }
}


static std::vector<Hooks::DummyCallback_t> s_glueXmlCharEnum;
void Hooks::GlueXML::registerCharEnum(DummyCallback_t func) { s_glueXmlCharEnum.push_back(func); }

static void LoadCharacters_bulk()
{
    for (auto func : s_glueXmlCharEnum)
        func();
}

static void (*LoadCharacters_orig)() = (decltype(LoadCharacters_orig))0x004E47E5;
static void __declspec(naked) LoadCharacters_hk()
{
    __asm {
        add esp, 8;
        pop esi;

        pushad;
        pushfd;
        call LoadCharacters_bulk;
        popfd;
        popad;

        ret;
    }
}

static bool isSpellReadied() { return ((DWORD)0x00D3F4E0 & 0x60) != 0 && (DWORD)0x00D3F4E4 != 0; }

static bool TerrainClick(float x, float y, float z) {
    TerrainClickEvent tc = {};
    tc.GUID = 0;
    tc.x = x;
    tc.y = y;
    tc.z = z;
    tc.button = 1;

    HandleTerrainClick_orig(&tc);
    return true;
}

static bool TraceLine(const C3Vector& start, const C3Vector& end, uint32_t hitFlags, C3Vector& intersectionPoint, float& completedBeforeIntersection) {
    completedBeforeIntersection = 1.0f;
    intersectionPoint = { 0.0f, 0.0f, 0.0f };

    uint8_t result = TraceLine_orig(
        const_cast<C3Vector*>(&start),
        const_cast<C3Vector*>(&end),
        &intersectionPoint,
        &completedBeforeIntersection,
        hitFlags,
        0
    );

    if (result != 0 && result != 1)
        return false;

    completedBeforeIntersection *= 100.0f;
    return static_cast<bool>(result);
}

static bool GetCursorWorldPosition(VecXYZ& worldPos) {
    Camera* camera = GetActiveCamera();
    if (!camera)
        return false;

    DWORD basePtr = *(DWORD*)UIBase;
    if (!basePtr)
        return false;

    float nx = *reinterpret_cast<float*>(basePtr + 4644) * 2.0f - 1.0f; // x perc
    float ny = *reinterpret_cast<float*>(basePtr + 4648) * 2.0f - 1.0f; // y perc

    float tanHalfFov = tanf(camera->fovInRadians * 0.3f);
    VecXYZ localRay = {
        nx * camera->aspect * tanHalfFov,
        ny * tanHalfFov,
        1.0f
    };

    const float* cameraMatrix = reinterpret_cast<const float*>(reinterpret_cast<uintptr_t>(camera) + 0x14);

    VecXYZ dir;
    dir.x = (-cameraMatrix[3]) * localRay.x + cameraMatrix[6] * localRay.y + cameraMatrix[0] * localRay.z;
    dir.y = (-cameraMatrix[4]) * localRay.x + cameraMatrix[7] * localRay.y + cameraMatrix[1] * localRay.z;
    dir.z = (-cameraMatrix[5]) * localRay.x + cameraMatrix[8] * localRay.y + cameraMatrix[2] * localRay.z;

    VecXYZ farPoint = {
        camera->pos.x + dir.x * MAX_TRACE_DISTANCE,
        camera->pos.y + dir.y * MAX_TRACE_DISTANCE,
        camera->pos.z + dir.z * MAX_TRACE_DISTANCE
    };

    C3Vector start = { camera->pos.x, camera->pos.y, camera->pos.z };
    C3Vector end = { farPoint.x, farPoint.y, farPoint.z };
    C3Vector hitPoint;
    float distance;

    bool hit = TraceLine(start, end, TERRAIN_HIT_FLAGS, hitPoint, distance);
    if (hit) {
        worldPos.x = hitPoint.X;
        worldPos.y = hitPoint.Y;
        worldPos.z = hitPoint.Z;
        return true;
    }

    return false;
}

static bool GetScreenSpaceSpellPosition(const C3Vector& playerPos, const C3Vector& cursorPos, float maxRange, C3Vector& outPos) {
    Camera* camera = GetActiveCamera();
    if (!camera)
        return false;

    const float* m = (float*)((uintptr_t)camera + 0x14);
    C3Vector right = { m[3], m[4], m[5] };
    C3Vector up = { m[6], m[7], m[8] };
    C3Vector fwd = { m[0], m[1], m[2] };

    C3Vector toCursor = {
        cursorPos.X - playerPos.X,
        cursorPos.Y - playerPos.Y,
        cursorPos.Z - playerPos.Z
    };

    float sx = dot(toCursor, right);
    float sy = dot(toCursor, up);
    float sz = dot(toCursor, fwd);

    double clampDistance = std::atoi(s_cvar_spellProjectionMaxRange->vStr);
    if (clampDistance > 0 && sqrtf(toCursor.X * toCursor.X + toCursor.Y * toCursor.Y + toCursor.Z * toCursor.Z) > maxRange + clampDistance)
        return false;

    float dx = cursorPos.X - playerPos.X;
    float dy = cursorPos.Y - playerPos.Y;
    float biasT = (sqrtf(dx * dx + dy * dy) - maxRange) / 25.0f;

    if (biasT < 0.0f)
        biasT = 0.0f;
    else if (biasT > 1.0f)
        biasT = 1.0f;
    sx *= (1.0f + std::atoi(s_cvar_spellProjectionHorizontalBias->vStr) * (biasT * biasT * (3.0f - 2.0f * biasT)));

    C3Vector dir = {
        sx * right.X + sy * up.X + sz * fwd.X,
        sx * right.Y + sy * up.Y + sz * fwd.Y,
        sx * right.Z + sy * up.Z + sz * fwd.Z
    };

    float dirLen = sqrtf(dir.X * dir.X + dir.Y * dir.Y + dir.Z * dir.Z);
    if (dirLen == 0.0f)
        return false;
    float invDirLen = 1.0f / dirLen;
    dir.X *= invDirLen; dir.Y *= invDirLen; dir.Z *= invDirLen;

    C3Vector perpA = (fabs(dir.Z) < 0.9f) ?
        C3Vector{ -dir.Y, dir.X, 0.0f } :
        C3Vector{ 0.0f, -dir.Z, dir.Y };

    float perpLen = sqrtf(perpA.X * perpA.X + perpA.Y * perpA.Y + perpA.Z * perpA.Z);
    float invPerpLen = 1.0f / (perpLen + 1e-6f);
    perpA.X *= invPerpLen; perpA.Y *= invPerpLen; perpA.Z *= invPerpLen;

    C3Vector perpB = {
        dir.Y * perpA.Z - dir.Z * perpA.Y,
        dir.Z * perpA.X - dir.X * perpA.Z,
        dir.X * perpA.Y - dir.Y * perpA.X
    };

    const float coneRadius = 0.25f;
    C3Vector coneOffsets[4] = {
        { perpA.X * coneRadius, perpA.Y * coneRadius, perpA.Z * coneRadius },
        { perpB.X * coneRadius, perpB.Y * coneRadius, perpB.Z * coneRadius },
        { -perpA.X * coneRadius, -perpA.Y * coneRadius, -perpA.Z * coneRadius },
        { -perpB.X * coneRadius, -perpB.Y * coneRadius, -perpB.Z * coneRadius }
    };

    C3Vector coneDirs[5];
    coneDirs[0] = dir;
    for (int i = 1; i < 5; ++i) {
        coneDirs[i] = {
            dir.X + coneOffsets[i - 1].X,
            dir.Y + coneOffsets[i - 1].Y,
            dir.Z + coneOffsets[i - 1].Z
        };
        float len = sqrtf(coneDirs[i].X * coneDirs[i].X + coneDirs[i].Y * coneDirs[i].Y + coneDirs[i].Z * coneDirs[i].Z);
        float invLen = 1.0f / (len + 1e-6f);
        coneDirs[i].X *= invLen;
        coneDirs[i].Y *= invLen;
        coneDirs[i].Z *= invLen;
    }

    C3Vector castDirs[6] = {
        { 0.0f, 0.0f, -1.0f },
        { 0.0f, 0.0f,  1.0f },
        { dir.X * 0.5f, dir.Y * 0.5f, -0.7071f },
        { -dir.X * 0.5f, -dir.Y * 0.5f, -0.7071f },
        { perpA.X * 0.5f, perpA.Y * 0.5f, -0.7071f },
        { -perpA.X * 0.5f, -perpA.Y * 0.5f, -0.7071f }
    };

    const float maxRangeSq = maxRange * maxRange;
    const float distStep = maxRange / 10.0f;

    C3Vector bestHit = playerPos;
    float bestDistSq = 0.0f;

    for (int distStepIdx = 10; distStepIdx >= 1; --distStepIdx) {
        float testDist = distStep * distStepIdx;

        for (int coneStep = 0; coneStep < 5; ++coneStep) {
            C3Vector testDir = coneDirs[coneStep];

            C3Vector testPoint = {
                playerPos.X + testDir.X * testDist,
                playerPos.Y + testDir.Y * testDist,
                playerPos.Z + testDir.Z * testDist
            };

            for (int castIdx = 0; castIdx < 6; ++castIdx) {
                C3Vector rayStart = {
                    testPoint.X - castDirs[castIdx].X * 50.0f,
                    testPoint.Y - castDirs[castIdx].Y * 50.0f,
                    testPoint.Z - castDirs[castIdx].Z * 50.0f
                };
                C3Vector rayEnd = {
                    testPoint.X + castDirs[castIdx].X * 100.0f,
                    testPoint.Y + castDirs[castIdx].Y * 100.0f,
                    testPoint.Z + castDirs[castIdx].Z * 100.0f
                };

                C3Vector hitPoint;
                float hitDist;
                if (TraceLine(rayStart, rayEnd, TERRAIN_HIT_FLAGS, hitPoint, hitDist)) {
                    float dx = hitPoint.X - playerPos.X;
                    float dy = hitPoint.Y - playerPos.Y;
                    float dz = hitPoint.Z - playerPos.Z;
                    float distSq = dx * dx + dy * dy + dz * dz;

                    if (distSq <= maxRangeSq && distSq > bestDistSq) {
                        if (distStepIdx == 10) {
                            outPos = hitPoint;
                            return true;
                        }
                        bestHit = hitPoint;
                        bestDistSq = distSq;
                    }
                }
            }
        }
    }

    if (bestDistSq) {
        outPos = bestHit;
    }
    else {
        outPos = {
            playerPos.X + dir.X * maxRange,
            playerPos.Y + dir.Y * maxRange,
            playerPos.Z + dir.Z * maxRange
        };
    }
    return true;
}


static int __cdecl SecureCmdOptionParse_hk(lua_State* L) {
    int result = SecureCmdOptionParse_orig(L);

    if (lua_gettop(L) < 3 || !lua_isstring(L, 2) || !lua_isstring(L, 3))
        return result;

    std::string_view parsed_target_view = lua_tostringnew(L, 3);
    bool is_cursor = std::equal(parsed_target_view.begin(), parsed_target_view.end(),
        "cursor", "cursor" + 6,
        [](char a, char b) { return std::tolower(a) == b; });
    bool is_playerlocation = std::equal(parsed_target_view.begin(), parsed_target_view.end(),
        "playerlocation", "playerlocation" + 14,
        [](char a, char b) { return std::tolower(a) == b; });

    if (!is_cursor && !is_playerlocation)
        return result;

    if (is_cursor) {
        g_cursorKeywordActive = true;
    }
    else if (is_playerlocation) {
        g_playerLocationKeywordActive = true;
    }

    std::string parsed_result = lua_tostringnew(L, 2);
    std::string orig_string = lua_tostringnew(L, 1);

    lua_pop(L, 3);
    lua_pushstring(L, orig_string.c_str());
    lua_pushstring(L, parsed_result.c_str());
    lua_pushnil(L);

    return result;
}

static void __cdecl HandleTerrainClick_hk(TerrainClickEvent* event)
{
    if (g_hasAdjustedPos) {
        if (isSpellReadied()) {
            event->x = g_adjustedSpellPos.X;
            event->y = g_adjustedSpellPos.Y;
            event->z = g_adjustedSpellPos.Z;
        }
        g_hasAdjustedPos = false;
    }
    HandleTerrainClick_orig(event);
}

static int __stdcall ProcessAoETargeting_hk(uint32_t* a1)
{
    g_hasAdjustedPos = false;

    CGUnit_C* player = ObjectMgr::GetCGUnitPlayer();
    if (!player)
        return ProcessAoETargeting_orig(a1);

    if (g_playerLocationKeywordActive) {
        C3Vector playerPos;
        player->GetPosition(playerPos);
        TerrainClick(playerPos.X, playerPos.Y, playerPos.Z);
        g_playerLocationKeywordActive = false;
        return 0;
    }

    C3Vector originalCursor = {
        *(float*)&a1[2],
        *(float*)&a1[3],
        *(float*)&a1[4]
    };

    if (!std::atoi(s_cvar_spellProjectionMode->vStr) == 1) {
        if (g_cursorKeywordActive) {
            TerrainClick(originalCursor.X, originalCursor.Y, originalCursor.Z);
            g_cursorKeywordActive = false;
            return 0;
        }
        return ProcessAoETargeting_orig(a1);
    }

    int spellContext = *(DWORD*)0x00D3F4E4 + 8;
    int spellObject = static_cast<int>(reinterpret_cast<intptr_t>(ObjectMgr::GetObjectPtr(*reinterpret_cast<uint64_t*>(spellContext + 8), 8)));
    if (spellObject) {
        float minRange = 0.0f;
        float maxRange = 0.0f;
        GetSpellRange_orig(spellObject, *(DWORD*)(spellContext + 24), &minRange, &maxRange, 0);
        if (maxRange > 0.0f) {
            C3Vector playerPos;
            player->GetPosition(playerPos);
            float dx = originalCursor.X - playerPos.X;
            float dy = originalCursor.Y - playerPos.Y;
            float dz = originalCursor.Z - playerPos.Z;

            if ((dx * dx + dy * dy + dz * dz) > ((maxRange * 0.98f) * (maxRange * 0.98f))) {
                C3Vector adjustedPos;
                if (GetScreenSpaceSpellPosition(playerPos, originalCursor, maxRange * 0.98f, adjustedPos)) {
                    if (g_cursorKeywordActive) {
                        TerrainClick(adjustedPos.X, adjustedPos.Y, adjustedPos.Z);
                        g_cursorKeywordActive = false;
                        return 0;
                    }
                    *(float*)&a1[2] = adjustedPos.X;
                    *(float*)&a1[3] = adjustedPos.Y;
                    *(float*)&a1[4] = adjustedPos.Z;

                    g_adjustedSpellPos = adjustedPos;
                    g_hasAdjustedPos = true;
                }
                else if (g_cursorKeywordActive) {
                    TerrainClick(originalCursor.X, originalCursor.Y, originalCursor.Z);
                    g_cursorKeywordActive = false;
                    return 0;
                }
            }
            else if (g_cursorKeywordActive) {
                TerrainClick(originalCursor.X, originalCursor.Y, originalCursor.Z);
                g_cursorKeywordActive = false;
                return 0;
            }
        }
    }
    return ProcessAoETargeting_orig(a1);
}


void Hooks::initialize()
{
    Hooks::FrameXML::registerCVar(&s_cvar_spellProjectionMode, "spellProjectionMode", NULL, (Console::CVarFlags)1, "1", CVarHandler_spellProjectionMode);
    Hooks::FrameXML::registerCVar(&s_cvar_spellProjectionMaxRange, "spellProjectionMaxRange", NULL, (Console::CVarFlags)1, "20", CVarHandler_spellProjectionMaxRange);
    Hooks::FrameXML::registerCVar(&s_cvar_spellProjectionHorizontalBias, "spellProjectionHorizontalBias", NULL, (Console::CVarFlags)1, "1.5", CVarHandler_spellProjectionHorizontalBias);
    DetourAttach(&(LPVOID&)CVars_Initialize_orig, CVars_Initialize_hk);
    DetourAttach(&(LPVOID&)FrameScript_FireOnUpdate_orig, FrameScript_FireOnUpdate_hk);
    DetourAttach(&(LPVOID&)FrameScript_FillEvents_orig, FrameScript_FillEvents_hk);
    DetourAttach(&(LPVOID&)Lua_OpenFrameXMLApi_orig, Lua_OpenFrameXMLApi_hk);
    DetourAttach(&(LPVOID&)GetGuidByKeyword_orig, GetGuidByKeyword_hk);
    DetourAttach(&(LPVOID&)GetKeywordsByGuid_orig, GetKeywordsByGuid_hk);
    DetourAttach(&(LPVOID&)LoadGlueXML_orig, LoadGlueXML_hk);
    DetourAttach(&(LPVOID&)LoadCharacters_orig, LoadCharacters_hk);
    DetourAttach(&(LPVOID&)SecureCmdOptionParse_orig, SecureCmdOptionParse_hk);
    DetourAttach(&(LPVOID&)HandleTerrainClick_orig, HandleTerrainClick_hk);
    DetourAttach(&(LPVOID&)ProcessAoETargeting_orig, ProcessAoETargeting_hk);
}

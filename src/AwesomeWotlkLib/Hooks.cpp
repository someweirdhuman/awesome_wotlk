#include "Hooks.h"
#include <Windows.h>
#include <Detours/detours.h>
#include <string>
#include <vector>
#include <unordered_map>
#include "Spell.h"

static Console::CVar* s_cvar_spellProjectionMode;
static int CVarHandler_spellProjectionMode(Console::CVar*, const char*, const char* value, LPVOID) { return 1; }

static constexpr float MAX_TRACE_DISTANCE = 1000.0f;
static constexpr uint32_t TERRAIN_HIT_FLAGS = 0x10111;
static bool g_cursorKeywordActive = false;
static int g_cursorSpellID = 0;
static bool g_playerLocationKeywordActive = false;
static bool collisionFound = false;
static C3Vector newTargetPos;

typedef int(__cdecl* SecureCmdOptionParse_t)(lua_State* L);
SecureCmdOptionParse_t SecureCmdOptionParse_orig = (SecureCmdOptionParse_t)0x00564AE0;

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

bool TraceLine(const C3Vector& start, const C3Vector& end, uint32_t hitFlags,
    C3Vector& intersectionPoint, float& completedBeforeIntersection) {
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

/*inline int __fastcall Spell_C_CancelPlayerSpells() {
    return ((decltype(&Spell_C_CancelPlayerSpells))0x00809AC0)();
}*/

bool GetAdjustedTargetPositionIfBlocked(const C3Vector& playerPos, const C3Vector& targetPos, float maxRange, C3Vector& outAdjustedPos) {
    C3Vector cameraPos = GetCameraPosC3();

    // direction from target to camera
    float dirX = cameraPos.X - targetPos.X;
    float dirY = cameraPos.Y - targetPos.Y;
    float dirZ = cameraPos.Z - targetPos.Z;

    float dirLength = std::sqrt(dirX * dirX + dirY * dirY + dirZ * dirZ);
    if (dirLength == 0.0f)
        return false;

    // normalize direction
    dirX /= dirLength;
    dirY /= dirLength;
    dirZ /= dirLength;

    // ray-cylinder intersection (vertical cylinder centered on playerPos, infinite in Z)
    float dx = dirX;
    float dy = dirY;

    float px = targetPos.X - playerPos.X;
    float py = targetPos.Y - playerPos.Y;

    float a = dx * dx + dy * dy;
    float b = 2.0f * (px * dx + py * dy);
    float c = px * px + py * py - maxRange * maxRange;

    float discriminant = b * b - 4.0f * a * c;
    if (discriminant < 0.0f || a == 0.0f)
        return false;

    float sqrtDisc = std::sqrt(discriminant);
    float t1 = (-b - sqrtDisc) / (2.0f * a);
    float t2 = (-b + sqrtDisc) / (2.0f * a);

    // cam direction (flat 2D vector from player to camera)
    float camDirX = cameraPos.X - playerPos.X;
    float camDirY = cameraPos.Y - playerPos.Y;
    float camLen = std::sqrt(camDirX * camDirX + camDirY * camDirY);
    if (camLen == 0.0f)
        return false;
    camDirX /= camLen;
    camDirY /= camLen;

    // pick the first one that lies in the "backward" half-space
    for (int i = 0; i < 2; ++i) {
        float t = (i == 0) ? t1 : t2;
        if (t < 0.0f)
            continue;

        float ix = targetPos.X + dirX * t;
        float iy = targetPos.Y + dirY * t;
        float iz = targetPos.Z + dirZ * t;

        // from player to intersection
        float vecX = ix - playerPos.X;
        float vecY = iy - playerPos.Y;
        float dot = vecX * camDirX + vecY * camDirY;

        if (dot < 0.0f) {
            //intersection is behind the player, relative to camera
            C3Vector hitPos = { ix, iy, iz };

            C3Vector upPos = { ix, iy, iz + maxRange };
            C3Vector downPos = { ix, iy, iz - maxRange };

            C3Vector upHit, downHit;
            float upFrac = 1.0f, downFrac = 1.0f;
            bool upHitFound = TraceLine(hitPos, upPos, 0x10111, upHit, upFrac);
            bool downHitFound = TraceLine(hitPos, downPos, 0x10111, downHit, downFrac);

            if (upHitFound && downHitFound) {
                float dUp = (upHit.X - ix) * (upHit.X - ix) +
                    (upHit.Y - iy) * (upHit.Y - iy) +
                    (upHit.Z - iz) * (upHit.Z - iz);
                float dDown = (downHit.X - ix) * (downHit.X - ix) +
                    (downHit.Y - iy) * (downHit.Y - iy) +
                    (downHit.Z - iz) * (downHit.Z - iz);
                outAdjustedPos = (dUp < dDown) ? upHit : downHit;
                return true;
            }
            else if (upHitFound) {
                outAdjustedPos = upHit;
                return true;
            }
            else if (downHitFound) {
                outAdjustedPos = downHit;
                return true;
            }

            outAdjustedPos = hitPos;
            return true;
        }
    }

    // no valid intersection
    return false;
}

static bool isSpellReadied() {
    unsigned int spellTargetingFlag = *(unsigned int*)0x00D3F4E4;
    unsigned int spellTargetingState = *(unsigned int*)0x00D3F4E0;
    return spellTargetingFlag != 0 && (spellTargetingState & 0x40) != 0;
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

static void onUpdateCallback() {
    if (!IsInWorld())
        return;

    if (g_cursorKeywordActive) {
        if (isSpellReadied()) {
            VecXYZ cursorPos;
            if (GetCursorWorldPosition(cursorPos)) {
                if (std::atoi(s_cvar_spellProjectionMode->vStr) == 1) {
                    CGUnit_C* player = ObjectMgr::GetCGUnitPlayer();
                    C3Vector playerPos;
                    player->GetPosition(playerPos);

                    float minRange = 0.0f;
                    float maxRange = 0.0f;

                    GetSpellRange_t GetSpellRange = reinterpret_cast<GetSpellRange_t>(0x00802C30);
                    GetSpellRange(player, g_cursorSpellID, &minRange, &maxRange, 0);

                    if (maxRange - 0.5f > 0.f) {
                        maxRange = maxRange - 0.5f;
                    }

                    C3Vector newOverrideTargetPos;
                    bool collisionFound = GetAdjustedTargetPositionIfBlocked(playerPos, reinterpret_cast<C3Vector&>(cursorPos), maxRange, newOverrideTargetPos);
                    if (collisionFound) {
                        TerrainClick(newOverrideTargetPos.X, newOverrideTargetPos.Y, newOverrideTargetPos.Z);
                    }
                    else {
                        TerrainClick(cursorPos.x, cursorPos.y, cursorPos.z);
                    }
                }
                else {
                    TerrainClick(cursorPos.x, cursorPos.y, cursorPos.z);
                }

                printf("%d \n", g_cursorSpellID);
            }
        }
        g_cursorKeywordActive = false;
        g_cursorSpellID = 0;
    }
    else if (g_playerLocationKeywordActive) {
        CGUnit_C* player = ObjectMgr::GetCGUnitPlayer();
        if (player && isSpellReadied()) {
            VecXYZ posPlayer;
            player->GetPosition(*reinterpret_cast<C3Vector*>(&posPlayer));
            TerrainClick(posPlayer.x, posPlayer.y, posPlayer.z);
        }
        g_playerLocationKeywordActive = false;
    }

    if (collisionFound) {
        collisionFound = false;
        newTargetPos.X = 0;
        newTargetPos.Y = 0;
        newTargetPos.Z = 0;
    }
}

double getGreenThingSize() {
    double v10 = ((double (*)())0x8019C0)();
    double v13;

    if (v10 >= 20.0)
        v13 = 20.0;
    else
        v13 = ((double (*)())0x8019C0)();

    return v13;
}

static int(__cdecl* original_project_texture)(int, int, int) = (int(__cdecl*)(int, int, int))0x004F8A40;
static int __cdecl hooked_project_texture(int a1, int a2, int a3) {
    if (std::atoi(s_cvar_spellProjectionMode->vStr) == 0) {
        return original_project_texture(a1, a2, a3);
    }
    auto* spellCast = *reinterpret_cast<SpellCast**>(0x00D3F4E4);
    if (!spellCast) {
        return original_project_texture(a1, a2, a3);
    }

    CGUnit_C* player = ObjectMgr::GetCGUnitPlayer();
    if (!player) {
        return original_project_texture(a1, a2, a3);
    }

    float minRange = 0.0f;
    float maxRange = 0.0f;

    GetSpellRange(player, spellCast->data.spellId, &minRange, &maxRange, 0);

    if (maxRange - 0.5f > 0.f) {
        maxRange = maxRange - 0.5f;
    }

    C3Vector playerPos;
    player->GetPosition(playerPos);

    C3Vector targetPos;
    targetPos.X = *(float*)0x00B74380;
    targetPos.Y = *(float*)0x00B74384;
    targetPos.Z = *(float*)0x00B74388;

    float dX = targetPos.X - playerPos.X;
    float dY = targetPos.Y - playerPos.Y;
    float dZ = targetPos.Z - playerPos.Z;
    float totalDist = std::sqrt(dX * dX + dY * dY + dZ * dZ);
    float dist = std::sqrt(dX * dX + dY * dY);
    if (totalDist < 0.0001f) {
        dX = 1.0f;
        dY = 0.0f;
        dist = 1.0f;
    }

    if (totalDist > maxRange && totalDist > 0.0001f) {
        collisionFound = GetAdjustedTargetPositionIfBlocked(playerPos, targetPos, maxRange, newTargetPos);
        if (collisionFound) {
            *(float*)0x00B74380 = newTargetPos.X;
            *(float*)0x00B74384 = newTargetPos.Y;
            *(float*)0x00B74388 = newTargetPos.Z;

            // revert projection texture/color to green again
            *(int*)0x00AC79A4 = 0;

            // revert projection size back to original
            *(float*)0xB74370 = static_cast<float>(getGreenThingSize());
        }
    }

    return original_project_texture(a1, a2, a3);
}

void __cdecl HandleTerrainClick_hook(TerrainClickEvent* event)
{
    if (collisionFound && std::atoi(s_cvar_spellProjectionMode->vStr) == 1) {
        event->x = newTargetPos.X;
        event->y = newTargetPos.Y;
        event->z = newTargetPos.Z;
    }

    HandleTerrainClick_orig(event);
}

typedef int(__cdecl* SpellCastFn)(int a1, int a2, int a3, int a4, int a5);
static SpellCastFn Spell_OnCastOriginal = (SpellCastFn)0x0080DA40;
int __cdecl Spell_OnCastHook(int spellId, int a2, int a3, int a4, int a5)
{
    int success = Spell_OnCastOriginal(spellId, a2, a3, a4, a5);
    if (success && Spell::IsForm(spellId))
    {
        CGUnit_C* player = ObjectMgr::GetCGUnitPlayer();
        if (player)
        {
            auto maybeForm = Spell::GetFormFromSpell(spellId);
            if (maybeForm.has_value())
            {
                Spell::ShapeshiftForm form = maybeForm.value();
                uint32_t formValue = static_cast<uint32_t>(form);

                player->SetValueBytes(UNIT_FIELD_BYTES_2, OFFSET_SHAPESHIFT_FORM, formValue);
            }
        }
    }

    if (g_cursorKeywordActive) {
        g_cursorSpellID = spellId;
    }

    return success;
}

void Hooks::initialize()
{
    Hooks::FrameScript::registerOnUpdate(onUpdateCallback);
    Hooks::FrameXML::registerCVar(&s_cvar_spellProjectionMode, "spellProjectionMode", NULL, (Console::CVarFlags)1, "0", CVarHandler_spellProjectionMode);
    DetourAttach(&(LPVOID&)CVars_Initialize_orig, CVars_Initialize_hk);
    DetourAttach(&(LPVOID&)FrameScript_FireOnUpdate_orig, FrameScript_FireOnUpdate_hk);
    DetourAttach(&(LPVOID&)FrameScript_FillEvents_orig, FrameScript_FillEvents_hk);
    DetourAttach(&(LPVOID&)Lua_OpenFrameXMLApi_orig, Lua_OpenFrameXMLApi_hk);
    DetourAttach(&(LPVOID&)GetGuidByKeyword_orig, GetGuidByKeyword_hk);
    DetourAttach(&(LPVOID&)GetKeywordsByGuid_orig, GetKeywordsByGuid_hk);
    DetourAttach(&(LPVOID&)LoadGlueXML_orig, LoadGlueXML_hk);
    DetourAttach(&(LPVOID&)LoadCharacters_orig, LoadCharacters_hk);
    DetourAttach(&(LPVOID&)SecureCmdOptionParse_orig, SecureCmdOptionParse_hk);
    DetourAttach(&(LPVOID&)Spell_OnCastOriginal, Spell_OnCastHook);
    DetourAttach((PVOID*)&original_project_texture, hooked_project_texture);
    DetourAttach(&(LPVOID&)HandleTerrainClick_orig, HandleTerrainClick_hook);
}

#include "NamePlates.h"
#include "GameClient.h"
#include "Hooks.h"
#include <Windows.h>
#include <Detours/detours.h>
#include <algorithm>
#include <vector>
#include <cstring>
#include <chrono>
#include <numeric>

static Console::CVar* s_cvar_nameplateDistance;
static Console::CVar* s_cvar_nameplateStacking;
static Console::CVar* s_cvar_nameplateXSpace;
static Console::CVar* s_cvar_nameplateYSpace;
static Console::CVar* s_cvar_nameplateUpperBorder;
static Console::CVar* s_cvar_nameplateOriginPos;
static Console::CVar* s_cvar_nameplateSpeedRaise;
static Console::CVar* s_cvar_nameplateSpeedReset;
static Console::CVar* s_cvar_nameplateSpeedLower;
static Console::CVar* s_cvar_nameplateHitboxHeight;
static Console::CVar* s_cvar_nameplateHitboxWidth;
static Console::CVar* s_cvar_nameplateFriendlyHitboxHeight;
static Console::CVar* s_cvar_nameplateFriendlyHitboxWidth;
static Console::CVar* s_cvar_nameplateStackFriendly;
static Console::CVar* s_cvar_nameplateMaxRaiseDistance;
static Console::CVar* s_cvar_nameplateExtendWorldFrameHeight;
static Console::CVar* s_cvar_nameplateUpperBorderOnlyBoss;
static Console::CVar* s_cvar_nameplateRecheckTreshold;

std::vector<NameplateHolder> Nameplates;

guid_t parseGuidFromString(const char* str)
{
    if (!str)
        return 0;

    return static_cast<guid_t>(strtoull(str, nullptr, 0));
}

static int C_NamePlate_GetNamePlateByGUID(lua_State* L)
{
    const char* guidStr = luaL_checkstring(L, 1);
    if (!guidStr) {
        lua_pushnil(L);
        return 1;
    }

    guid_t guid = parseGuidFromString(guidStr);
    if (!guid) return 0;

    Frame* nameplate = getNameplateByGuid(guid);
    if (!nameplate)
        return 0;

    lua_pushframe(L, nameplate);
    return 1;
}

static int C_NamePlate_GetNamePlateTokenByGUID(lua_State* L)
{
    const char* guidStr = luaL_checkstring(L, 1);
    if (!guidStr) {
        lua_pushnil(L);
        return 1;
    }

    guid_t guid = parseGuidFromString(guidStr);
    if (!guid) return 0;

    for (size_t i = 0; i < Nameplates.size(); ++i) {
        Frame* frame = Nameplates[i].nameplate;
        if (!frame)
            continue;

        if (frame->guid == guid)
        {
            static char token[32];
            snprintf(token, sizeof(token), "nameplate%zu", i);
            lua_pushstring(L, token);
            return 1;
        }
    }

    lua_pushnil(L);
    return 1;
}

static guid_t getTokenGuid(int id)
{
    if (id < 0 || id >= Nameplates.size())
        return 0;
    if (Nameplates[id].nameplate == nullptr)
        return 0;
    return Nameplates[id].nameplate->guid;
}

int getTokenId(guid_t guid)
{
    if (!guid)
        return -1;

    for (size_t i = 0; i < Nameplates.size(); i++) {
        Frame* frame = Nameplates[i].nameplate;
        if (!frame)
            continue;

        if (frame->guid == guid && Nameplates[i].visible == true)
        {
            return static_cast<int>(i);
        }
    }

    return -1;
}

static int CVarHandler_NameplateDistance(Console::CVar*, const char*, const char* value, LPVOID)
{
    double f = atof(value);
    f = f > 0.f ? f : 41.f;
    *(float*)0x00ADAA7C = (float)(f * f);
    return 1;
}
std::chrono::steady_clock::time_point gLastCallTime = std::chrono::steady_clock::now();
static int CVarHandler_NameplateXSpace(Console::CVar*, const char*, const char* value, LPVOID) { return 1; }
static int CVarHandler_NameplateYSpace(Console::CVar*, const char*, const char* value, LPVOID) { return 1; }
static int CVarHandler_NameplateUpperBorder(Console::CVar*, const char*, const char* value, LPVOID) { return 1; }
static int CVarHandler_NameplateOriginPos(Console::CVar*, const char*, const char* value, LPVOID) { return 1; }
static int CVarHandler_NameplateSpeedRaise(Console::CVar*, const char*, const char* value, LPVOID) { return 1; }
static int CVarHandler_NameplateSpeedLower(Console::CVar*, const char*, const char* value, LPVOID) { return 1; }
static int CVarHandler_NameplateSpeedReset(Console::CVar*, const char*, const char* value, LPVOID) { return 1; }
static int CVarHandler_NameplateHitboxHeight(Console::CVar*, const char*, const char* value, LPVOID) { return 1; }
static int CVarHandler_NameplateHitboxWidth(Console::CVar*, const char*, const char* value, LPVOID) { return 1; }
static int CVarHandler_NameplateFriendlyHitboxHeight(Console::CVar*, const char*, const char* value, LPVOID) { return 1; }
static int CVarHandler_NameplateFriendlyHitboxWidth(Console::CVar*, const char*, const char* value, LPVOID) { return 1; }
static int CVarHandler_NameplateMaxRaiseDistance(Console::CVar*, const char*, const char* value, LPVOID) { return 1; }
static int CVarHandler_NameplateUpperBorderOnlyBoss(Console::CVar*, const char*, const char* value, LPVOID) { return 1; }
static int CVarHandler_NameplateRecheckTreshold(Console::CVar*, const char*, const char* value, LPVOID) { return 1; }
static int CVarHandler_NameplateExtendWorldFrameHeight(Console::CVar*, const char*, const char* value, LPVOID) {
    if (!IsInWorld()) return 1;

    bool enabled = atoi(value) == 1 ? true : false;
    ConfigureWorldFrame(GetLuaState(), enabled);
    return 1;
}
static int CVarHandler_NameplateStackFriendly(Console::CVar*, const char* name, const char* value, LPVOID){ return 1; }

static int CVarHandler_NameplateStacking(Console::CVar*, const char*, const char* value, LPVOID)
{
    lua_State* L = GetLuaState();

    for (NameplateHolder& holder : Nameplates)
    {
        if (holder.nameplate)
        {
            lua_pushframe(L, holder.nameplate);
            int frame_idx = lua_gettop(L);
            holder.xpos = 0.f;
            holder.ypos = 0.f;
            SetClampedToScreen(L, frame_idx, false);
            SetClampRectInsets(L, frame_idx, 0, 0, 0, 0);
            lua_pop(L, 1);
        }
    }

    if (Console::CVar* cvar = Console::FindCVar("nameplateAllowOverlap"); cvar)
        Console::SetCVarValue(cvar, "1", 1, 0, 0, 1);

    return 1;
}

static int C_NamePlate_GetNamePlates(lua_State* L)
{
    lua_createtable(L, 0, 0);
    int id = 1;
    for (NameplateHolder& holder : Nameplates)
    {
        if ((holder.visible) && holder.nameplate)
        {
            lua_pushframe(L, holder.nameplate);
            lua_rawseti(L, -2, id++);
        }
    }
    return 1;
}

static int C_NamePlate_GetNamePlateForUnit(lua_State* L)
{
    const char* token = luaL_checkstring(L, 1);
    guid_t guid = ObjectMgr::GetGuidByUnitID(token);

    if (!guid) return 0;
    Frame* nameplate = getNameplateByGuid(guid);

    if (!nameplate) return 0;

    lua_pushframe(L, nameplate);
    return 1;
}

static int lua_openlibnameplates(lua_State* L)
{
    luaL_Reg methods[] = {
        {"GetNamePlates", C_NamePlate_GetNamePlates},
        {"GetNamePlateForUnit", C_NamePlate_GetNamePlateForUnit},
        {"GetNamePlateByGUID", C_NamePlate_GetNamePlateByGUID},
        {"GetNamePlateTokenByGUID", C_NamePlate_GetNamePlateTokenByGUID},
    };

    lua_createtable(L, 0, std::size(methods));
    for (size_t i = 0; i < std::size(methods); i++) {
        lua_pushcfunction(L, methods[i].func);
        lua_setfield(L, -2, methods[i].name);
    }
    lua_setglobal(L, "C_NamePlate");
    return 0;
}

static void NameplateStackingUpdateSmooth()
{
    lua_State* L = GetLuaState();

    double scale = 0;
    GetEffectiveScale(L, scale);

    int nameplateHitboxHeight = std::atof(s_cvar_nameplateHitboxHeight->vStr) * scale;
    int nameplateHitboxWidth = std::atof(s_cvar_nameplateHitboxWidth->vStr) * scale;
    int nameplateFriendlyHitboxHeight = std::atof(s_cvar_nameplateFriendlyHitboxHeight->vStr) * scale;
    int nameplateFriendlyHitboxWidth = std::atof(s_cvar_nameplateFriendlyHitboxWidth->vStr) * scale;

    bool nameplateStackFriendly = std::atoi(s_cvar_nameplateStackFriendly->vStr) == 1;
    auto now = std::chrono::steady_clock::now();
    double delta = std::chrono::duration<float>(now - gLastCallTime).count();
    gLastCallTime = now;

    int xSpace = std::atoi(s_cvar_nameplateXSpace->vStr);
    int ySpace = std::atoi(s_cvar_nameplateYSpace->vStr);
    double speedRaise = std::atof(s_cvar_nameplateSpeedRaise->vStr) * 2.0;
    double speedLower = std::atof(s_cvar_nameplateSpeedLower->vStr) * 2.0;

    int upperBorder = std::atoi(s_cvar_nameplateUpperBorder->vStr);
    int originPos = std::atoi(s_cvar_nameplateOriginPos->vStr);
    double maxRaise = std::atof(s_cvar_nameplateMaxRaiseDistance->vStr);
    bool onlyBossUpper = std::atoi(s_cvar_nameplateUpperBorderOnlyBoss->vStr) == 1;
    int worldFrameExtend = std::atoi(s_cvar_nameplateExtendWorldFrameHeight->vStr);
    double recheckThreshold = std::atof(s_cvar_nameplateRecheckTreshold->vStr);

    std::vector<size_t> active_indices;
    for (size_t i = 0; i < Nameplates.size(); ++i) {
        if (Nameplates[i].nameplate == nullptr) continue;

        lua_pushframe(L, Nameplates[i].nameplate);
        int frame_idx = lua_gettop(L);

        CGUnit_C* unit = (CGUnit_C * )ObjectMgr::Get(Nameplates[i].nameplate->guid, TYPEMASK_UNIT);

        Nameplates[i].isFriendly = IsFriendlyByReaction(unit);
        Nameplates[i].rank = unit->GetCreatureRank();

        if (Nameplates[i].isFriendly) {
            if (nameplateFriendlyHitboxHeight > 0) SetHeight(L, frame_idx, nameplateFriendlyHitboxHeight);
            if (nameplateFriendlyHitboxWidth > 0) SetWidth(L, frame_idx, nameplateFriendlyHitboxWidth);
        }
        else {
            if (nameplateHitboxHeight > 0) SetHeight(L, frame_idx, nameplateHitboxHeight);
            if (nameplateHitboxWidth > 0) SetWidth(L, frame_idx, nameplateHitboxWidth);
        }

        std::string point, relativeToName, relativePoint;
        double xOfs, yOfs;
        bool status = GetPoint(L, frame_idx, 1, point, relativeToName, relativePoint, xOfs, yOfs);
        if (status) {
            Nameplates[i].xpos = xOfs;
            Nameplates[i].ypos = yOfs;
        }

        double halfWidth = 0, halfHeight = 0;
        GetSize(L, frame_idx, halfWidth, halfHeight);
        halfWidth *= 0.5, halfHeight *= 0.5;

        SetClampedToScreen(L, frame_idx, true);
        SetClampRectInsets(L, frame_idx, halfWidth, -halfWidth, -halfHeight, -Nameplates[i].ypos - originPos + halfHeight);

        if ((Nameplates[i].visible) && (nameplateStackFriendly || Nameplates[i].isFriendly)) {
            active_indices.push_back(i);
        }else {
            Nameplates[i].targetStackOffset = 0.0;
            Nameplates[i].currentStackOffset = 0.0;
        }

        lua_pop(L, 1);
    }

    std::sort(active_indices.begin(), active_indices.end(), [&](size_t a, size_t b) {
        const auto& pA = Nameplates[a];
        const auto& pB = Nameplates[b];

        if (std::abs(pA.ypos - pB.ypos) > 8.0) {
            return pA.ypos < pB.ypos;
        }
        return pA.nameplate->guid < pB.nameplate->guid;
    });

    for (size_t i = 0; i < active_indices.size(); ++i) {
        NameplateHolder& p1 = Nameplates[active_indices[i]];
        double testOffset = 0.0;

        for (int attempts = 0; attempts < 10; ++attempts) {
            bool collisionFound = false;
            double currentY1 = p1.ypos + testOffset;

            for (size_t j = 0; j < i; ++j) {
                NameplateHolder& p2 = Nameplates[active_indices[j]];

                if (std::abs(p1.xpos - p2.xpos) < (xSpace - 2)) {
                    double currentY2 = p2.ypos + p2.targetStackOffset;

                    if (std::abs(currentY1 - currentY2) < (ySpace - 1)) {
                        testOffset = (currentY2 + ySpace) - p1.ypos;
                        collisionFound = true;
                        break;
                    }
                }
            }
            if (!collisionFound) break;
        }

        if (maxRaise > 0 && testOffset > maxRaise) {
            testOffset = maxRaise;
        }

        if (std::abs(testOffset - p1.targetStackOffset) > recheckThreshold || testOffset == 0) {
            p1.targetStackOffset = testOffset;
        }
    }

    for (size_t idx : active_indices) {
        NameplateHolder& nameplate = Nameplates[idx];
        lua_pushframe(L, nameplate.nameplate);
        int frame_idx = lua_gettop(L);

        double width, height;
        GetSize(L, frame_idx, width, height);

        double diff = nameplate.targetStackOffset - nameplate.currentStackOffset;

        if (std::abs(diff) < 0.5) {
            nameplate.currentStackOffset = nameplate.targetStackOffset;
        }
        else {
            double lerpFactor = (diff > 0 ? speedRaise : speedLower) * delta;
            if (lerpFactor > 0.8) lerpFactor = 0.8;
            nameplate.currentStackOffset += diff * lerpFactor;
        }

        SetClampedToScreen(L, frame_idx, true);

        int currentUpper = (onlyBossUpper && nameplate.rank != 3) ? -worldFrameExtend : upperBorder;
        float bottomInset = -nameplate.ypos - nameplate.currentStackOffset - originPos + (height / 2);

        SetClampRectInsets(L, frame_idx, -10, 10, currentUpper, bottomInset);
        lua_pop(L, 1);
    }
}

static void OnGameShutdown()
{
    Nameplates.clear();
}

static void onUpdateCallback()
{
    if (!IsInWorld()) return;

    Camera* camera = GetActiveCamera();
    VecXYZ camPos = camera->pos;

    std::vector<std::pair<NameplateHolder*, VecXYZ>> sorted;
    sorted.reserve(Nameplates.size());

    for (auto& h : Nameplates)
    {
        if (!h.visible || !h.nameplate)
            continue;

        Unit* u = (Unit*)ObjectMgr::Get(h.nameplate->guid, TYPEMASK_UNIT);
        if (!u)
            continue;

        VecXYZ pos;
        u->vmt->GetPosition(u, &pos);
        sorted.emplace_back(&h, pos);
    }

    std::sort(sorted.begin(), sorted.end(),
        [&](const auto& a, const auto& b)
        {
            return camPos.distance(a.second) < camPos.distance(b.second);
        }
    );

    int level = 0;
    for (auto& [h, pos] : sorted)
        CFrame::SetFrameLevel(h->nameplate, level += 10, 1);
 
    if (strcmp(s_cvar_nameplateStacking->vStr, "1") == 0) {
        NameplateStackingUpdateSmooth();
    }

}

LPVOID PatchNamePlateLevelUpdate_orig = (LPVOID)0x0098E9F9;
void __declspec(naked) PatchNamePlateLevelUpdate_hk()
{
    __asm {
        push edi;
        push 0x0098EA27;
        ret;
    }
}

static void OnEnterWorld()
{
    bool enabled = std::atoi(s_cvar_nameplateExtendWorldFrameHeight->vStr) == 1;
    ConfigureWorldFrame(GetLuaState(), enabled);
}

static void WriteJump(void* src, void* dst)
{
    DWORD old;
    VirtualProtect(src, 5, PAGE_EXECUTE_READWRITE, &old);

    uintptr_t rel = (uintptr_t)dst - (uintptr_t)src - 5;
    *(BYTE*)src = 0xE9;
    *(DWORD*)((uintptr_t)src + 1) = (DWORD)rel;

    VirtualProtect(src, 5, old, &old);
}

void InstallDetour(const uintptr_t target, const int stolen, void* detour, void** trampoline)
{
    *trampoline = VirtualAlloc(nullptr, stolen + 5,
        MEM_COMMIT | MEM_RESERVE,
        PAGE_EXECUTE_READWRITE);

    memcpy(*trampoline, (void*)target, stolen);

    BYTE* tramp = (BYTE*)(*trampoline);
    if (tramp[0] == 0xE8) {
        int32_t origRel = *(int32_t*)(tramp + 1);
        uintptr_t origCallTarget = target + 5 + origRel;
        *(int32_t*)(tramp + 1) = (int32_t)(origCallTarget - ((uintptr_t)tramp + 5));
    }

    uintptr_t trampJmp = (uintptr_t)(*trampoline) + stolen;
    *(BYTE*)trampJmp = 0xE9;
    *(DWORD*)(trampJmp + 1) = (DWORD)((target + stolen) - trampJmp - 5);

    WriteJump((void*)target, detour);
}

static void* showNameplateTrampoline = nullptr;
static void* hideNameplateTrampoline = nullptr;
static void* createNameplateTrampoline = nullptr;
static void* postNameplateTrampoline = nullptr;
static void* destroyUnitTrampoline = nullptr;
std::vector<std::function<void()>> NameplateQueue;

void OnNameplateShow(Unit* unit)
{
    Frame* nameplate = unit->nameplate;

    if (nameplate == nullptr) return;

    auto it = std::find_if(Nameplates.begin(), Nameplates.end(),
    [nameplate](NameplateHolder& h)
    {
        return h.nameplate == nameplate;
    });

    int index = -1;
    bool shouldFireEvent = false;

    if (it == Nameplates.end())
    {
        // Try to find an empty slot
        auto emptyIt = std::find_if(Nameplates.begin(), Nameplates.end(),
        [](NameplateHolder& h)
        {
            return h.nameplate == nullptr;
        });

        if (emptyIt != Nameplates.end())
        {
            emptyIt->nameplate = nameplate;
            emptyIt->visible = true;
            index = (int)std::distance(Nameplates.begin(), emptyIt) + 1;
            shouldFireEvent = true;
        }
        else
        {
            // No empty slot, add to end
            Nameplates.push_back({ nameplate, true });
            index = (int)Nameplates.size();
            shouldFireEvent = true;
        }
    }

    if (shouldFireEvent)
    {
        NameplateQueue.push_back([index]()
        {
            char token[16];
            snprintf(token, sizeof(token), "nameplate%d", index);
            FrameScript::FireEvent(NAME_PLATE_UNIT_ADDED, "%s", token);
        });
    }
}

void OnNameplateHide(Unit* unit)
{
    Frame* nameplate = unit->nameplate;

    auto it = std::find_if(
        Nameplates.begin(),
        Nameplates.end(),
        [&](const NameplateHolder& h)
        {
            if (h.nameplate == nullptr)
                return false;
            return h.nameplate == nameplate;
        }
    );

    if (it != Nameplates.end())
    {
        int index = (int)std::distance(Nameplates.begin(), it) + 1;

        //need to fire event before setting nameplate to nullptr so ingame u can check stuff
        char token[16];
        snprintf(token, sizeof(token), "nameplate%d", index);
        FrameScript::FireEvent(NAME_PLATE_UNIT_REMOVED, "%s", token);

        it->visible = false;
        it->nameplate = nullptr;
        it->ypos = 0.f;
        it->xpos = 0.f;
        it->xposOffset = 0.f;
        it->currentStackOffset = 0.f;
        it->targetStackOffset = 0.f;
    }
}

void OnNameplateCreate(Unit* unit)
{
    Frame* nameplate = unit->nameplate;

    NameplateQueue.push_back([nameplate]()
    {
        lua_State* L = GetLuaState();
        lua_pushstring(L, NAME_PLATE_CREATED);
        lua_pushframe(L, nameplate);
        FrameScript::FireEvent_inner(
            FrameScript::GetEventIdByName(NAME_PLATE_CREATED),
            L,
            2
        );
        lua_pop(L, 2);
    });
}

void OnPostNameplate()
{
    for (auto& fn : NameplateQueue)
        fn();

    NameplateQueue.clear();
}

void onDestroyUnit(Unit* unit)
{
    Frame* nameplate = unit->nameplate;

    auto it = std::find_if(
        Nameplates.begin(),
        Nameplates.end(),
        [&](const NameplateHolder& h)
        {
            if (h.nameplate == nullptr)
                return false;
            return h.nameplate == nameplate;
        }
    );

    if (it != Nameplates.end())
    {
        int index = (int)std::distance(Nameplates.begin(), it) + 1;
        it->visible = false;
        it->nameplate = nullptr;
        it->ypos = 0.f;
        it->xpos = 0.f;
        it->xposOffset = 0.f;
        it->currentStackOffset = 0.f;
        it->targetStackOffset = 0.f;
    }
}


__declspec(naked) void detourShowNameplate()
{
    __asm {
        pushad
        pushfd

        mov eax, esi
        push eax
        call OnNameplateShow
        add esp, 4

        popfd
        popad

        jmp showNameplateTrampoline
    }
}

__declspec(naked) void detourHideNameplate()
{
    __asm {
        pushad
        pushfd

        mov eax, esi
        push eax
        call OnNameplateHide
        add esp, 4

        popfd
        popad

        jmp hideNameplateTrampoline
    }
}

__declspec(naked) void detourCreateNameplate()
{
    __asm {
        pushad
        pushfd

        mov eax, esi
        push eax
        call OnNameplateCreate
        add esp, 4

        popfd
        popad

        jmp createNameplateTrampoline
    }
}

__declspec(naked) void detourPostNameplate()
{
    __asm {
        pushad
        pushfd

        call OnPostNameplate

        popfd
        popad

        jmp postNameplateTrampoline
    }
}

__declspec(naked) void detourUnitDestroy()
{
    __asm {
        pushad
        pushfd

        mov eax, ecx
        push eax
        call onDestroyUnit
        add esp, 4

        popfd
        popad

        jmp destroyUnitTrampoline
    }
}


void NamePlates::initialize()
{
    AllocConsole();

    // Redirect stdout to the new console
    FILE* fp;
    freopen_s(&fp, "CONOUT$", "w", stdout);
    freopen_s(&fp, "CONOUT$", "w", stderr);
    freopen_s(&fp, "CONIN$", "r", stdin);

    Hooks::FrameXML::registerLuaLib(lua_openlibnameplates);
    Hooks::FrameXML::registerEvent(NAME_PLATE_CREATED);
    Hooks::FrameXML::registerEvent(NAME_PLATE_UNIT_ADDED);
    Hooks::FrameXML::registerEvent(NAME_PLATE_UNIT_REMOVED);
    Hooks::FrameXML::registerEvent(NAME_PLATE_OWNER_CHANGED);
    Hooks::FrameXML::registerCVar(&s_cvar_nameplateDistance, "nameplateDistance", NULL, (Console::CVarFlags)1, "43", CVarHandler_NameplateDistance);
    Hooks::FrameXML::registerCVar(&s_cvar_nameplateStacking, "nameplateStacking", NULL, (Console::CVarFlags)1, "0", CVarHandler_NameplateStacking);
    Hooks::FrameXML::registerCVar(&s_cvar_nameplateXSpace, "nameplateXSpace", NULL, (Console::CVarFlags)1, "130", CVarHandler_NameplateXSpace);
    Hooks::FrameXML::registerCVar(&s_cvar_nameplateYSpace, "nameplateYSpace", NULL, (Console::CVarFlags)1, "20", CVarHandler_NameplateYSpace);
    Hooks::FrameXML::registerCVar(&s_cvar_nameplateUpperBorder, "nameplateUpperBorder", NULL, (Console::CVarFlags)1, "30", CVarHandler_NameplateUpperBorder);
    Hooks::FrameXML::registerCVar(&s_cvar_nameplateOriginPos, "nameplateOriginPos", NULL, (Console::CVarFlags)1, "20", CVarHandler_NameplateOriginPos);
    Hooks::FrameXML::registerCVar(&s_cvar_nameplateSpeedRaise, "nameplateSpeedRaise", NULL, (Console::CVarFlags)1, "1", CVarHandler_NameplateSpeedRaise);
    Hooks::FrameXML::registerCVar(&s_cvar_nameplateSpeedReset, "nameplateSpeedReset", NULL, (Console::CVarFlags)1, "1", CVarHandler_NameplateSpeedReset);
    Hooks::FrameXML::registerCVar(&s_cvar_nameplateSpeedLower, "nameplateSpeedLower", NULL, (Console::CVarFlags)1, "1", CVarHandler_NameplateSpeedLower);
    Hooks::FrameXML::registerCVar(&s_cvar_nameplateHitboxHeight, "nameplateHitboxHeight", NULL, (Console::CVarFlags)1, "0", CVarHandler_NameplateHitboxHeight);
    Hooks::FrameXML::registerCVar(&s_cvar_nameplateHitboxWidth, "nameplateHitboxWidth", NULL, (Console::CVarFlags)1, "0", CVarHandler_NameplateHitboxWidth);
    Hooks::FrameXML::registerCVar(&s_cvar_nameplateFriendlyHitboxHeight, "nameplateFriendlyHitboxHeight", NULL, (Console::CVarFlags)1, "0", CVarHandler_NameplateFriendlyHitboxHeight);
    Hooks::FrameXML::registerCVar(&s_cvar_nameplateFriendlyHitboxWidth, "nameplateFriendlyHitboxWidth", NULL, (Console::CVarFlags)1, "0", CVarHandler_NameplateFriendlyHitboxWidth);
    Hooks::FrameXML::registerCVar(&s_cvar_nameplateStackFriendly, "nameplateStackFriendly", NULL, (Console::CVarFlags)1, "1", CVarHandler_NameplateStackFriendly);
    Hooks::FrameXML::registerCVar(&s_cvar_nameplateMaxRaiseDistance, "nameplateMaxRaiseDistance", NULL, (Console::CVarFlags)1, "200", CVarHandler_NameplateMaxRaiseDistance);
    Hooks::FrameXML::registerCVar(&s_cvar_nameplateExtendWorldFrameHeight, "nameplateExtendWorldFrameHeight", NULL, (Console::CVarFlags)1, "0", CVarHandler_NameplateExtendWorldFrameHeight);
    Hooks::FrameXML::registerCVar(&s_cvar_nameplateUpperBorderOnlyBoss, "nameplateUpperBorderOnlyBoss", NULL, (Console::CVarFlags)1, "0", CVarHandler_NameplateUpperBorderOnlyBoss);
    Hooks::FrameXML::registerCVar(&s_cvar_nameplateRecheckTreshold, "nameplateRecheckTreshold", NULL, (Console::CVarFlags)1, "0", CVarHandler_NameplateRecheckTreshold);

    InstallDetour(0x0072B38C, 5, &detourHideNameplate, &hideNameplateTrampoline);
    InstallDetour(0x0072582C, 5, &detourShowNameplate, &showNameplateTrampoline);
    InstallDetour(0x00725766, 5, &detourCreateNameplate, &createNameplateTrampoline);
    InstallDetour(0x004F90B6, 5, &detourPostNameplate, &postNameplateTrampoline);
    InstallDetour(0x00737BA0, 6, &detourUnitDestroy, &destroyUnitTrampoline);

    Hooks::FrameScript::registerToken("nameplate", getTokenGuid, getTokenId);
    Hooks::FrameScript::registerOnUpdate(onUpdateCallback);
    Hooks::FrameScript::registerOnEnter(OnEnterWorld);
    Hooks::FrameScript::registerOnGameShutdown(OnGameShutdown);
    DetourAttach(&(LPVOID&)PatchNamePlateLevelUpdate_orig, PatchNamePlateLevelUpdate_hk);
}
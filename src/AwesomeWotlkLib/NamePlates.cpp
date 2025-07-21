#include "NamePlates.h"
#include "GameClient.h"
#include "Hooks.h"
#include <Windows.h>
#include <Detours/detours.h>
#include <algorithm>
#include <vector>
#include <cstring>
#include <chrono>
#define NAME_PLATE_CREATED "NAME_PLATE_CREATED"
#define NAME_PLATE_UNIT_ADDED "NAME_PLATE_UNIT_ADDED"
#define NAME_PLATE_UNIT_REMOVED "NAME_PLATE_UNIT_REMOVED"
#define NAME_PLATE_OWNER_CHANGED "NAME_PLATE_OWNER_CHANGED"


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
static Console::CVar* s_cvar_nameplateStackFriendly;
static Console::CVar* s_cvar_nameplateStackFriendlyMode;


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

    NamePlateEntry* entry = getEntryByGuid(guid);
    if (!entry || !entry->nameplate)
        return 0;

    lua_pushframe(L, entry->nameplate);
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

    NamePlateVars& vars = lua_findorcreatevars(GetLuaState());

    for (size_t i = 0; i < vars.nameplates.size(); ++i)
    {
        const NamePlateEntry& entry = vars.nameplates[i];
        if ((entry.flags & NamePlateFlag_CreatedAndVisible) == NamePlateFlag_CreatedAndVisible &&
            entry.guid == guid)
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
    NamePlateVars& vars = lua_findorcreatevars(GetLuaState());
    if (id >= vars.nameplates.size())
        return 0;
    return vars.nameplates[id].guid;
}

int getTokenId(guid_t guid)
{
    if (!guid) return -1;
    NamePlateVars& vars = lua_findorcreatevars(GetLuaState());
    for (size_t i = 0; i < vars.nameplates.size(); i++) {
        NamePlateEntry& entry = vars.nameplates[i];
        if ((entry.flags & NamePlateFlag_CreatedAndVisible) == NamePlateFlag_CreatedAndVisible && entry.guid == guid)
            return i;
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
static void UpdateNameplateFriendliness(const char* name, const char* value)
{
    if (!IsInWorld()) return;

    lua_State* L = GetLuaState();
    NamePlateVars& vars = lua_findorcreatevars(L);

    bool nameplateStackFriendly = std::atoi(name == "NameplateStackFriendly" ? value : s_cvar_nameplateStackFriendly->vStr) == 1;
    uint8_t nameplateStackFriendlyMode = static_cast<uint8_t>(std::atoi(name == "NameplateStackFriendlyMode" ? value : s_cvar_nameplateStackFriendlyMode->vStr));

    for (size_t i = 0; i < vars.nameplates.size(); ++i)
    {
        NamePlateEntry& entry = vars.nameplates[i];

        char token[16];
        snprintf(token, sizeof(token), "nameplate%zu", i + 1);

        CGUnit_C* unit = (CGUnit_C*)ObjectMgr::Get(token, TYPEMASK_UNIT);
        if (!unit)
            continue;

        lua_pushframe(L, entry.nameplate);
        int frame_idx = lua_gettop(L);

        if (nameplateStackFriendlyMode == 0) {
            entry.isFriendly = ObjectMgr::GetCGUnitPlayer()->UnitReaction(unit) > 2;
        }
        else {
            entry.isFriendly = IsFriendlyByColor(L, frame_idx) == 5;
        }

        entry.xpos = 0.f;
        entry.ypos = 0.f;
        entry.position = 0.f;

        SetClampedToScreen(L, frame_idx, false);
        SetClampRectInsets(L, frame_idx, 0, 0, 0, 0);
        lua_pop(L, 1);
    }
}

static int CVarHandler_NameplateStackFriendly(Console::CVar*, const char* name, const char* value, LPVOID)
{
    UpdateNameplateFriendliness(name, value);
    return 1;
}

static int CVarHandler_NameplateStackFriendlyMode(Console::CVar*, const char* name, const char* value, LPVOID)
{
    UpdateNameplateFriendliness(name, value);
    return 1;
}
static int CVarHandler_NameplateStacking(Console::CVar*, const char*, const char* value, LPVOID)
{
    lua_State* L = GetLuaState();
    NamePlateVars& vars = lua_findorcreatevars(L);
    for (size_t i = 0; i < vars.nameplates.size(); ++i)
    {
        NamePlateEntry& entry = vars.nameplates[i];

        lua_pushframe(L, entry.nameplate);
        int frame_idx = lua_gettop(L);
        entry.xpos = 0.f;
        entry.ypos = 0.f; 
        entry.position = 0.f;
        SetClampedToScreen(L, frame_idx, false);
        SetClampRectInsets(L, frame_idx, 0, 0, 0, 0);
        lua_pop(L, 1);
    }

    //always set this to 1
    if (Console::CVar* cvar = Console::FindCVar("nameplateAllowOverlap"); cvar)
        Console::SetCVarValue(cvar, "1", 1, 0, 0, 1);

    return 1;
}

static int C_NamePlate_GetNamePlates(lua_State* L)
{
    lua_createtable(L, 0, 0);
    NamePlateVars& vars = lua_findorcreatevars(L);
    int id = 1;
    for (NamePlateEntry& entry : vars.nameplates) {
        if ((entry.flags & NamePlateFlag_Visible) && entry.guid) {
            lua_pushframe(L, entry.nameplate);
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
    NamePlateEntry* entry = getEntryByGuid(guid);
    if (!entry) return 0;
    lua_pushframe(L, entry->nameplate);
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

static void NameplateStackingUpdate(lua_State* L, NamePlateVars* vars)
{
    bool nameplateStackFriendly = std::atoi(s_cvar_nameplateStackFriendly->vStr) == 1 ? true : false;
    auto now = std::chrono::steady_clock::now();
    double delta = std::chrono::duration<float>(now - gLastCallTime).count() * 500;  // delta 
    gLastCallTime = now;

    for (size_t i = 0; i < vars->nameplates.size(); ++i)
    {
        NamePlateEntry& entry = vars->nameplates[i];
        if (!nameplateStackFriendly && entry.isFriendly) {
            continue;
        }
        std::string point;
        std::string relativeToName;
        std::string relativePoint;
        double xOfs, yOfs;

        lua_pushframe(L, entry.nameplate);
        int frame_idx = lua_gettop(L);

        if (entry.flags & NamePlateFlag_Visible) {
            bool status = GetPoint(L, frame_idx, 1, point, relativeToName, relativePoint, xOfs, yOfs);
            if (status) {
                entry.xpos = xOfs;
                entry.ypos = yOfs;
            }
        }
        else {
            entry.xpos = 0.f;
            entry.ypos = 0.f;
            SetClampedToScreen(L, frame_idx, false);
            SetClampRectInsets(L, frame_idx, 0, 0, 0, 0);
        }
        lua_pop(L, 1);
    }

    int xSpace = std::atoi(s_cvar_nameplateXSpace->vStr);
    int ySpace = std::atoi(s_cvar_nameplateYSpace->vStr);
    int upperBorder = std::atoi(s_cvar_nameplateUpperBorder->vStr);
    int originPos = std::atoi(s_cvar_nameplateOriginPos->vStr);
    int speedRaise = std::atoi(s_cvar_nameplateSpeedRaise->vStr);
    int speedReset = std::atoi(s_cvar_nameplateSpeedReset->vStr);
    int speedLower = std::atoi(s_cvar_nameplateSpeedLower->vStr);
    int nameplateHitboxHeight = std::atoi(s_cvar_nameplateHitboxHeight->vStr);
    int nameplateHitboxWidth = std::atoi(s_cvar_nameplateHitboxWidth->vStr);


    for (size_t i = 0; i < vars->nameplates.size(); ++i) {
        NamePlateEntry& nameplate_1 = vars->nameplates[i];

        if (!nameplateStackFriendly && nameplate_1.isFriendly) {
            continue;
        }

        lua_pushframe(L, nameplate_1.nameplate);
        int frame_1 = lua_gettop(L);

        double width = 0, height = 0;
        GetSize(L, frame_1, width, height);
        if (nameplateHitboxHeight > 0) {
            SetHeight(L, frame_1, nameplateHitboxHeight);
        }

        if (nameplateHitboxWidth > 0) {
            SetWidth(L, frame_1, nameplateHitboxWidth);
        }

        if (nameplate_1.flags & NamePlateFlag_Visible) {
            double min = 1000;
            bool reset = true;
            for (size_t j = 0; j < vars->nameplates.size(); ++j) {
                NamePlateEntry& nameplate_2 = vars->nameplates[j];
                lua_pushframe(L, nameplate_2.nameplate);

                int frame_2 = lua_gettop(L);
                if (nameplate_2.flags & NamePlateFlag_Visible) {
                    if (nameplate_1.guid != nameplate_2.guid) {
                        double xdiff = nameplate_1.xpos - nameplate_2.xpos;
                        double ydiff = nameplate_1.ypos + nameplate_1.position - nameplate_2.ypos - nameplate_2.position;
                        double ydiff_origin = nameplate_1.ypos - nameplate_2.ypos - nameplate_2.position;

                        if (std::abs(xdiff) < xSpace) {
                            if (ydiff >= 0 && std::abs(ydiff) < min) {
                                min = std::abs(ydiff);
                            }
                            if (std::abs(ydiff_origin) < ySpace + 2 * delta) {
                                reset = false;
                            }
                        }
                    }
                }
                lua_pop(L, 1);
            }

            double oldposition = nameplate_1.position;

            if (oldposition >= 2 * delta && reset == 1) {
                nameplate_1.position = oldposition - std::exp(-10.0f / oldposition) * delta * speedReset;
            }
            else if (min < ySpace) {
                nameplate_1.position = oldposition + std::exp(-min / ySpace) * delta * speedRaise;
            }
            else if ((oldposition >= 2 * delta) && (min > ySpace + delta * 2)) {
                nameplate_1.position = oldposition - std::exp(-ySpace / min) * delta * 0.8f * speedLower;
            }

            SetClampedToScreen(L, frame_1, true);
            SetClampRectInsets(L, frame_1, -10, 10, upperBorder, -nameplate_1.ypos - nameplate_1.position - originPos + height);
        }

        lua_pop(L, 1);
    }
}

static void onUpdateCallback()
{
    if (!IsInWorld()) return;

    static std::vector<std::tuple<Frame*, guid_t, float>> s_plateSort;

    lua_State* L = GetLuaState();
    NamePlateVars& vars = lua_findorcreatevars(L);

    if (strcmp(s_cvar_nameplateStacking->vStr, "1") == 0) {
        NameplateStackingUpdate(L, &vars);
    }

    Player* player = ObjectMgr::GetPlayer();
    VecXYZ posPlayer;
    if (player) player->ToUnit()->vmt->GetPosition(player->ToUnit(), &posPlayer);

    uint8_t nameplateStackFriendlyMode = static_cast<uint8_t>(std::atoi(s_cvar_nameplateStackFriendlyMode->vStr));

    ObjectMgr::EnumObjects([&vars, player, &posPlayer, &nameplateStackFriendlyMode, &L](guid_t guid) -> bool {
        Unit* unit = (Unit*)ObjectMgr::Get(guid, TYPEMASK_UNIT);
        if (!unit || !unit->nameplate) return true;
        auto it = std::find_if(vars.nameplates.begin(), vars.nameplates.end(), [nameplate = unit->nameplate](const NamePlateEntry& entry) {
            return entry.nameplate == nameplate;
        });
        if (it == vars.nameplates.end()) {
            NamePlateEntry& entry = vars.nameplates.emplace_back();
            entry.guid = guid;
            entry.nameplate = unit->nameplate;
            entry.updateId = vars.updateId;
        } else {
            if (it->guid != guid) {
                it->guid = guid;
                if (it->flags & NamePlateFlag_Visible) {
                    char token[16];
                    snprintf(token, std::size(token), "nameplate%lu", std::distance(vars.nameplates.begin(), it) + 1);
                    CGUnit_C* unit = (CGUnit_C*)ObjectMgr::Get(token, TYPEMASK_UNIT);
                    if (unit) {
                        lua_pushframe(L, it->nameplate);

                        int frame_idx = lua_gettop(L);
                        if (nameplateStackFriendlyMode == 0) {
                            it->isFriendly = ObjectMgr::GetCGUnitPlayer()->UnitReaction(unit) > 2 ? true : false;
                        } else {
                            it->isFriendly = IsFriendlyByColor(L, frame_idx) == 5;
                        }
                        lua_pop(L, 1);
                    }
                    FrameScript::FireEvent(NAME_PLATE_OWNER_CHANGED, "%s", token);
                }
            }
            it->updateId = vars.updateId;
        }

        if (player) {
            VecXYZ unitPos;
            unit->vmt->GetPosition(unit, &unitPos);
            s_plateSort.push_back({ unit->nameplate, guid, posPlayer.distance(unitPos) });
        }
        
        return true;
    });

    if (!s_plateSort.empty()) {
        std::sort(s_plateSort.begin(), s_plateSort.end(), [targetGuid = ObjectMgr::GetTargetGuid()](auto& a1, auto& a2) {
            auto& [frame1, guid1, distance1] = a1;
            auto& [frame2, guid2, distance2] = a2;
            if (guid1 == targetGuid) return false;
            if (guid2 == targetGuid) return true;
            return distance1 > distance2;
        });

        int level = 10;
        for (auto& entry : s_plateSort)
            CFrame::SetFrameLevel(std::get<0>(entry), level++, 1);

        s_plateSort.clear();
    }

    for (size_t i = 0; i < vars.nameplates.size(); i++) {
        NamePlateEntry& entry = vars.nameplates[i];
        if (entry.updateId == vars.updateId) {
            if (!(entry.flags & NamePlateFlag_Created)) {
                lua_pushstring(L, NAME_PLATE_CREATED); // tbl, event
                lua_pushframe(L, entry.nameplate); // tbl,  event, frame
                FrameScript::FireEvent_inner(FrameScript::GetEventIdByName(NAME_PLATE_CREATED), L, 2); // tbl
                lua_pop(L, 2);
                entry.flags |= NamePlateFlag_Created;
            }

            if (!(entry.flags & NamePlateFlag_Visible)) {
                entry.flags |= NamePlateFlag_Visible;
                char token[16];
                snprintf(token, std::size(token), "nameplate%d", i + 1);
                FrameScript::FireEvent(NAME_PLATE_UNIT_ADDED, "%s", token);
                CGUnit_C* unit = (CGUnit_C*)ObjectMgr::Get(token, TYPEMASK_UNIT);
                if (unit) {
                    lua_pushframe(L, entry.nameplate);
                    int frame_idx = lua_gettop(L);
                    if (nameplateStackFriendlyMode == 0) {
                        entry.isFriendly = ObjectMgr::GetCGUnitPlayer()->UnitReaction(unit) > 2 ? true : false;
                    }
                    else {
                        entry.isFriendly = IsFriendlyByColor(L, frame_idx) == 5;
                    }
                    lua_pop(L, 1);
                }
            }
        } else {
            if (entry.flags & NamePlateFlag_Visible) {
                char token[16];
                snprintf(token, std::size(token), "nameplate%d", i + 1);
                FrameScript::FireEvent(NAME_PLATE_UNIT_REMOVED, "%s", token);
                entry.guid = 0;
                entry.flags &= ~NamePlateFlag_Visible;
                entry.position = 0;
                entry.xpos = 0;
                entry.ypos = 0;
                entry.isFriendly = false;
            }
        }
    }

    vars.updateId++;
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

void NamePlates::initialize()
{
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
    Hooks::FrameXML::registerCVar(&s_cvar_nameplateStackFriendly, "nameplateStackFriendly", NULL, (Console::CVarFlags)1, "0", CVarHandler_NameplateStackFriendly);
    Hooks::FrameXML::registerCVar(&s_cvar_nameplateStackFriendlyMode, "nameplateStackFriendlyMode", NULL, (Console::CVarFlags)1, "0", CVarHandler_NameplateStackFriendlyMode);
    Hooks::FrameScript::registerToken("nameplate", getTokenGuid, getTokenId);
    Hooks::FrameScript::registerOnUpdate(onUpdateCallback);

    DetourAttach(&(LPVOID&)PatchNamePlateLevelUpdate_orig, PatchNamePlateLevelUpdate_hk);
}
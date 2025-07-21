#include "UnitAPI.h"
#include "GameClient.h"
#include "Hooks.h"
#include "Nameplates.h"

extern int getTokenId(guid_t guid);


static int lua_UnitIsControlled(lua_State* L)
{
    Unit* unit = (Unit*)ObjectMgr::Get(luaL_checkstring(L, 1), ObjectFlags_Unit);
    if (!unit || !(unit->entry->flags & (UNIT_FLAG_FLEEING | UNIT_FLAG_CONFUSED | UNIT_FLAG_STUNNED | UNIT_FLAG_PACIFIED)))
        return 0;
    lua_pushnumber(L, 1);
    return 1;
}

static int lua_UnitIsDisarmed(lua_State* L)
{
    Unit* unit = (Unit*)ObjectMgr::Get(luaL_checkstring(L, 1), ObjectFlags_Unit);
    if (!unit || !(unit->entry->flags & UNIT_FLAG_DISARMED))
        return 0;
    lua_pushnumber(L, 1);
    return 1;
}

static int lua_UnitIsSilenced(lua_State* L)
{
    Unit* unit = (Unit*)ObjectMgr::Get(luaL_checkstring(L, 1), ObjectFlags_Unit);
    if (!unit || !(unit->entry->flags & UNIT_FLAG_SILENCED))
        return 0;
    lua_pushnumber(L, 1);
    return 1;
}
static int lua_UnitOccupations(lua_State* L)
{
    Unit* unit = (Unit*)ObjectMgr::Get(luaL_checkstring(L, 1), ObjectFlags_Unit);
    if (!unit)
        return 0;
    lua_pushnumber(L, unit->entry->npc_flags);
    return 1;
}

static int lua_UnitOwner(lua_State* L)
{
    Unit* unit = (Unit*)ObjectMgr::Get(luaL_checkstring(L, 1), ObjectFlags_Unit);
    if (!unit)
        return 0;
    guid_t ownerGuid = unit->entry->summonedBy ? unit->entry->summonedBy : unit->entry->createdBy;
    if (!ownerGuid)
        return 0;

    lua_pushstring(L, ObjectMgr::PlayerNameFromGuid(ownerGuid));
    return 1;
}

static int lua_UnitTokenFromGUID(lua_State* L)
{
    guid_t guid = ObjectMgr::HexString2Guid(luaL_checkstring(L, 1));
    if (!guid || !((Unit*)ObjectMgr::Get(guid, ObjectFlags_Unit)))
        return 0;

    Unit* player = (Unit*)ObjectMgr::Get("player", ObjectFlags_Unit);
    if (player && ObjectMgr::GetGuidByUnitID("player") == guid) {
        lua_pushstring(L, "player");
        return 1;
    }

    Unit* vehicle = (Unit*)ObjectMgr::Get("vehicle", ObjectFlags_Unit);
    if (vehicle && ObjectMgr::GetGuidByUnitID("vehicle") == guid) {
        lua_pushstring(L, "vehicle");
        return 1;
    }

    Unit* pet = (Unit*)ObjectMgr::Get("pet", ObjectFlags_Unit);
    if (pet && ObjectMgr::GetGuidByUnitID("pet") == guid) {
        lua_pushstring(L, "pet");
        return 1;
    }

    for (int i = 1; i <= 4; i++) {
        char token[16];
        snprintf(token, sizeof(token), "party%d", i);
        Unit* partyUnit = (Unit*)ObjectMgr::Get(token, ObjectFlags_Unit);
        if (partyUnit && ObjectMgr::GetGuidByUnitID(token) == guid) {
            lua_pushstring(L, token);
            return 1;
        }
    }

    for (int i = 1; i <= 4; i++) {
        char token[16];
        snprintf(token, sizeof(token), "partypet%d", i);
        Unit* partyPet = (Unit*)ObjectMgr::Get(token, ObjectFlags_Unit);
        if (partyPet && ObjectMgr::GetGuidByUnitID(token) == guid) {
            lua_pushstring(L, token);
            return 1;
        }
    }

    for (int i = 1; i <= 40; i++) {
        char token[16];
        snprintf(token, sizeof(token), "raid%d", i);
        Unit* raidUnit = (Unit*)ObjectMgr::Get(token, ObjectFlags_Unit);
        if (raidUnit && ObjectMgr::GetGuidByUnitID(token) == guid) {
            lua_pushstring(L, token);
            return 1;
        }
    }

    for (int i = 1; i <= 40; i++) {
        char token[16];
        snprintf(token, sizeof(token), "raidpet%d", i);
        Unit* raidPet = (Unit*)ObjectMgr::Get(token, ObjectFlags_Unit);
        if (raidPet && ObjectMgr::GetGuidByUnitID(token) == guid) {
            lua_pushstring(L, token);
            return 1;
        }
    }
    
    int tokenId = getTokenId(guid);
    if (tokenId > 0) {
        char token[16];
        snprintf(token, sizeof(token), "nameplate%d", tokenId + 1);
        lua_pushstring(L, token);
        return 1;
    }

    for (int i = 1; i <= 5; i++) {
        char token[16];
        snprintf(token, sizeof(token), "arena%d", i);
        Unit* arenaUnit = (Unit*)ObjectMgr::Get(token, ObjectFlags_Unit);
        if (arenaUnit && ObjectMgr::GetGuidByUnitID(token) == guid) {
            lua_pushstring(L, token);
            return 1;
        }
    }

    for (int i = 1; i <= 5; i++) {
        char token[16];
        snprintf(token, sizeof(token), "arenapet%d", i);
        Unit* arenaPet = (Unit*)ObjectMgr::Get(token, ObjectFlags_Unit);
        if (arenaPet && ObjectMgr::GetGuidByUnitID(token) == guid) {
            lua_pushstring(L, token);
            return 1;
        }
    }

    for (int i = 1; i <= 5; i++) {
        char token[16];
        snprintf(token, sizeof(token), "boss%d", i);
        Unit* bossUnit = (Unit*)ObjectMgr::Get(token, ObjectFlags_Unit);
        if (bossUnit && ObjectMgr::GetGuidByUnitID(token) == guid) {
            lua_pushstring(L, token);
            return 1;
        }
    }

    Unit* target = (Unit*)ObjectMgr::Get("target", ObjectFlags_Unit);
    if (target && ObjectMgr::GetGuidByUnitID("target") == guid) {
        lua_pushstring(L, "target");
        return 1;
    }

    Unit* focus = (Unit*)ObjectMgr::Get("focus", ObjectFlags_Unit);
    if (focus && ObjectMgr::GetGuidByUnitID("focus") == guid) {
        lua_pushstring(L, "focus");
        return 1;
    }

    /*
    Unit* npc = (Unit*)ObjectMgr::Get("npc", ObjectFlags_Unit);
    if (npc && ObjectMgr::GetGuidByUnitID("npc") == guid) {
        lua_pushstring(L, "npc");
        return 1;
    }
    */

    Unit* mouseover = (Unit*)ObjectMgr::Get("mouseover", ObjectFlags_Unit);
    if (mouseover && ObjectMgr::GetGuidByUnitID("mouseover") == guid) {
        lua_pushstring(L, "mouseover");
        return 1;
    }

    /*
    Unit* softenemy = (Unit*)ObjectMgr::Get("softenemy", ObjectFlags_Unit);
    if (softenemy && ObjectMgr::GetGuidByUnitID("softenemy") == guid) {
        lua_pushstring(L, "softenemy");
        return 1;
    }

    Unit* softfriend = (Unit*)ObjectMgr::Get("softfriend", ObjectFlags_Unit);
    if (softfriend && ObjectMgr::GetGuidByUnitID("softfriend") == guid) {
        lua_pushstring(L, "softfriend");
        return 1;
    }

    Unit* softinteract = (Unit*)ObjectMgr::Get("softinteract", ObjectFlags_Unit);
    if (softinteract && ObjectMgr::GetGuidByUnitID("softinteract") == guid) {
        lua_pushstring(L, "softinteract");
        return 1;
    }
    */

    return 0;
}

static int lua_openunitlib(lua_State* L)
{
    luaL_Reg funcs[] = {
        { "UnitIsControlled", lua_UnitIsControlled },
        { "UnitIsDisarmed", lua_UnitIsDisarmed },
        { "UnitIsSilenced", lua_UnitIsSilenced },
        { "UnitOccupations", lua_UnitOccupations },
        { "UnitOwner", lua_UnitOwner },
        { "UnitTokenFromGUID", lua_UnitTokenFromGUID },
    };

    for (auto& [name, func] : funcs) {
        lua_pushcfunction(L, func);
        lua_setglobal(L, name);
    }

    return 0;
}

void UnitAPI::initialize()
{
    Hooks::FrameXML::registerLuaLib(lua_openunitlib);
}
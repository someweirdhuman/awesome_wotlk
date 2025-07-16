#include "UnitAPI.h"
#include "GameClient.h"
#include "Hooks.h"
#include "Nameplates.h"

const uintptr_t nameStore = 0x00C5D938 + 0x8;
static const char* PlayerNameFromGuid(guid_t guid) {
    Unit* owner = (Unit*)ObjectMgr::Get(guid, ObjectFlags_Unit);
    uint32_t unitType = *(uint32_t*)((uintptr_t)owner + 0x14);

    if (unitType == 0x4) {
        uint32_t mask = *(uint32_t*)(nameStore + 0x24);
        uint32_t base = *(uint32_t*)(nameStore + 0x1C);

        if (base == 0)
            return "Unknown";

        uint32_t shortGUID = guid & 0xffffffff;
        uint32_t offset = 12 * (mask & shortGUID);

        uint32_t current = *(uint32_t*)(base + offset + 8);
        uint32_t offsetValue = *(uint32_t*)(base + offset);

        if ((current & 0x1) == 0x1 || current == 0)
            return "Unknown";

        uint32_t testGUID = *(uint32_t*)(current);

        int iterations = 0;
        while (testGUID != shortGUID && iterations < 10) {
            iterations++;
            current = *(uint32_t*)(current + offsetValue + 4);

            if ((current & 0x1) == 0x1 || current == 0)
                return "Unknown";

            testGUID = *(uint32_t*)(current);
        }

        if (iterations >= 10)
            return "Unknown";

        return (char*)(current + 0x20);
    }
    return (char*)*(uint32_t*)(*(uint32_t*)((uintptr_t)owner + 0x964) + 0x05C);
}

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

    char guidStr[32];
    snprintf(guidStr, sizeof(guidStr), "0x%llx", ownerGuid);
    lua_pushstring(L, PlayerNameFromGuid(ownerGuid));
    lua_pushstring(L, guidStr);

    return 2;
}

static int lua_UnitTokenFromGUID(lua_State* L)
{
    guid_t guid = ObjectMgr::HexString2Guid(luaL_checkstring(L, 1));
    if (!guid || !((Unit*)ObjectMgr::Get(guid, ObjectFlags_Unit)))
        return 0;

    Unit* player = (Unit*)ObjectMgr::Get("player", ObjectFlags_Unit);
    if (player && *(uint32_t*)((char*)player + 0x30) == guid) {
        lua_pushstring(L, "player");
        return 1;
    }

    Unit* vehicle = (Unit*)ObjectMgr::Get("vehicle", ObjectFlags_Unit);
    if (vehicle && *(uint32_t*)((char*)vehicle + 0x30) == guid) {
        lua_pushstring(L, "vehicle");
        return 1;
    }

    Unit* pet = (Unit*)ObjectMgr::Get("pet", ObjectFlags_Unit);
    if (pet && *(uint32_t*)((char*)pet + 0x30) == guid) {
        lua_pushstring(L, "pet");
        return 1;
    }

    for (int i = 1; i <= 4; i++) {
        char token[16];
        snprintf(token, sizeof(token), "party%d", i);
        Unit* partyUnit = (Unit*)ObjectMgr::Get(token, ObjectFlags_Unit);
        if (partyUnit && *(uint32_t*)((char*)partyUnit + 0x30) == guid) {
            lua_pushstring(L, token);
            return 1;
        }
    }

    for (int i = 1; i <= 4; i++) {
        char token[16];
        snprintf(token, sizeof(token), "partypet%d", i);
        Unit* partyPet = (Unit*)ObjectMgr::Get(token, ObjectFlags_Unit);
        if (partyPet && *(uint32_t*)((char*)partyPet + 0x30) == guid) {
            lua_pushstring(L, token);
            return 1;
        }
    }

    for (int i = 1; i <= 40; i++) {
        char token[16];
        snprintf(token, sizeof(token), "raid%d", i);
        Unit* raidUnit = (Unit*)ObjectMgr::Get(token, ObjectFlags_Unit);
        if (raidUnit && *(uint32_t*)((char*)raidUnit + 0x30) == guid) {
            lua_pushstring(L, token);
            return 1;
        }
    }

    for (int i = 1; i <= 40; i++) {
        char token[16];
        snprintf(token, sizeof(token), "raidpet%d", i);
        Unit* raidPet = (Unit*)ObjectMgr::Get(token, ObjectFlags_Unit);
        if (raidPet && *(uint32_t*)((char*)raidPet + 0x30) == guid) {
            lua_pushstring(L, token);
            return 1;
        }
    }

    NamePlateEntry* entry = getEntryByGuid(guid);
    if (entry) {
        NamePlateVars& vars = lua_findorcreatevars(GetLuaState());
        auto it = std::find_if(vars.nameplates.begin(), vars.nameplates.end(), [nameplate = entry->nameplate](const NamePlateEntry& entry) {
            return entry.nameplate == nameplate;
            });
        char token[16];
        snprintf(token, std::size(token), "nameplate%lu", std::distance(vars.nameplates.begin(), it) + 1);
        lua_pushstring(L, token);
        return 1;
    }

    for (int i = 1; i <= 5; i++) {
        char token[16];
        snprintf(token, sizeof(token), "arena%d", i);
        Unit* arenaUnit = (Unit*)ObjectMgr::Get(token, ObjectFlags_Unit);
        if (arenaUnit && *(uint32_t*)((char*)arenaUnit + 0x30) == guid) {
            lua_pushstring(L, token);
            return 1;
        }
    }

    for (int i = 1; i <= 5; i++) {
        char token[16];
        snprintf(token, sizeof(token), "arenapet%d", i);
        Unit* arenaPet = (Unit*)ObjectMgr::Get(token, ObjectFlags_Unit);
        if (arenaPet && *(uint32_t*)((char*)arenaPet + 0x30) == guid) {
            lua_pushstring(L, token);
            return 1;
        }
    }

    for (int i = 1; i <= 5; i++) {
        char token[16];
        snprintf(token, sizeof(token), "boss%d", i);
        Unit* bossUnit = (Unit*)ObjectMgr::Get(token, ObjectFlags_Unit);
        if (bossUnit && *(uint32_t*)((char*)bossUnit + 0x30) == guid) {
            lua_pushstring(L, token);
            return 1;
        }
    }

    Unit* target = (Unit*)ObjectMgr::Get("target", ObjectFlags_Unit);
    if (target && *(uint32_t*)((char*)target + 0x30) == guid) {
        lua_pushstring(L, "target");
        return 1;
    }

    Unit* focus = (Unit*)ObjectMgr::Get("focus", ObjectFlags_Unit);
    if (focus && *(uint32_t*)((char*)focus + 0x30) == guid) {
        lua_pushstring(L, "focus");
        return 1;
    }

    /*
    Unit* npc = (Unit*)ObjectMgr::Get("npc", ObjectFlags_Unit);
    if (npc && npc->guid == guid) {
        lua_pushstring(L, "npc");
        return 1;
    }
    */

    Unit* mouseover = (Unit*)ObjectMgr::Get("mouseover", ObjectFlags_Unit);
    if (mouseover && *(uint32_t*)((char*)mouseover + 0x30) == guid) {
        lua_pushstring(L, "mouseover");
        return 1;
    }

    /*
    Unit* softenemy = (Unit*)ObjectMgr::Get("softenemy", ObjectFlags_Unit);
    if (softenemy && *(uint32_t*)((char*)softenemy + 0x30) == guid) {
        lua_pushstring(L, "softenemy");
        return 1;
    }

    Unit* softfriend = (Unit*)ObjectMgr::Get("softfriend", ObjectFlags_Unit);
    if (softfriend && *(uint32_t*)((char*)softfriend + 0x30) == guid) {
        lua_pushstring(L, "softfriend");
        return 1;
    }

    Unit* softinteract = (Unit*)ObjectMgr::Get("softinteract", ObjectFlags_Unit);
    if (softinteract && *(uint32_t*)((char*)softinteract + 0x30) == guid) {
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
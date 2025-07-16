#pragma once

#include <Windows.h>
#include <Detours/detours.h>
#include <algorithm>
#include <vector>
#include <cstring>
#include "Hooks.h"

namespace NamePlates {
    void initialize();
}

// Forward declarations if needed
struct Frame;
using guid_t = uint64_t;  // Or your actual definition

// Flag type
using NamePlateFlags = uint32_t;

// Enum for flags
enum NamePlateFlag_ {
    NamePlateFlag_Null = 0,
    NamePlateFlag_Created = (1 << 0),
    NamePlateFlag_Visible = (1 << 1),
    NamePlateFlag_CreatedAndVisible = NamePlateFlag_Created | NamePlateFlag_Visible,
};

// Entry struct
struct NamePlateEntry {
    NamePlateEntry() : nameplate(0), guid(0), flags(NamePlateFlag_Null), updateId(0) {}
    Frame* nameplate;
    guid_t guid;
    NamePlateFlags flags;
    uint32_t updateId;
};

// Main state struct
struct NamePlateVars {
    NamePlateVars() : updateId(1) {}
    std::vector<NamePlateEntry> nameplates;
    uint32_t updateId;
};

inline NamePlateVars& lua_findorcreatevars(lua_State* L)
{
    struct Dummy {
        static int lua_gc(lua_State* L)
        {
            ((NamePlateVars*)lua_touserdata(L, -1))->~NamePlateVars();
            return 0;
        }
    };
    NamePlateVars* result = NULL;
    lua_getfield(L, LUA_REGISTRYINDEX, "nameplatevars"); // vars
    if (lua_isuserdata(L, -1))
        result = (NamePlateVars*)lua_touserdata(L, -1);
    lua_pop(L, 1);

    if (!result) {
        result = (NamePlateVars*)lua_newuserdata(L, sizeof(NamePlateVars)); // vars
        new (result) NamePlateVars();

        lua_createtable(L, 0, 1); // vars, mt
        lua_pushcfunction(L, Dummy::lua_gc); // vars, mt, gc
        lua_setfield(L, -2, "__gc"); // vars, mt
        lua_setmetatable(L, -2); // vars
        lua_setfield(L, LUA_REGISTRYINDEX, "nameplatevars");
    }
    return *result;
}

inline NamePlateEntry* getEntryByGuid(guid_t guid)
{
    if (!guid) return NULL;
    NamePlateVars& vars = lua_findorcreatevars(GetLuaState());
    auto it = std::find_if(vars.nameplates.begin(), vars.nameplates.end(), [guid](const NamePlateEntry& entry) {
        return (entry.flags & NamePlateFlag_Visible) && entry.guid == guid;
        });
    return it != vars.nameplates.end() ? &(*it) : NULL;
}
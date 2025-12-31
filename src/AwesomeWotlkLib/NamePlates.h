#pragma once

#include <Windows.h>
#include <Detours/detours.h>
#include <algorithm>
#include <vector>
#include <cstring>
#include "Hooks.h"
#include <string>
#include <format>
#define NAME_PLATE_CREATED "NAME_PLATE_CREATED"
#define NAME_PLATE_UNIT_ADDED "NAME_PLATE_UNIT_ADDED"
#define NAME_PLATE_UNIT_REMOVED "NAME_PLATE_UNIT_REMOVED"
#define NAME_PLATE_OWNER_CHANGED "NAME_PLATE_OWNER_CHANGED"

struct NameplateHolder
{
    Frame* nameplate;
    bool visible = false;

    int index;

    //stacking stuff, helpers more or less
    bool isFriendly = false;
    uint8_t rank = 0;

    //stacking stuff
    double ypos;
    double xpos;
    double xposOffset;
    double currentStackOffset;
    double targetStackOffset;
};

extern std::unordered_map<int, NameplateHolder> Nameplates; // index -> holder
extern std::unordered_map<Frame*, int> FrameToIndex;        // fast lookup
extern int NextIndex;
using namespace std;

namespace NamePlates {
    void initialize();
}

// Forward declarations if needed
struct Frame
{
    char pad0[32];
    void* idk;

    char pad1[672 - 36];
    Frame* next;

    char pad_between[4];
    guid_t guid;

    char pad2[71];
};
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
    bool isFriendly = false;
    uint8_t rank = 0;

    double ypos;
    double xpos;
    double xposOffset;
    double currentStackOffset; 
    double targetStackOffset;  
};

inline bool GetEffectiveScale(lua_State* L, double& scale)
{
    // Get UIParent
    lua_getglobal(L, "UIParent");

    // Get scale: UIParent:GetEffectiveScale()
    lua_getfield(L, -1, "GetEffectiveScale");
    lua_pushvalue(L, -2); // push UIParent as self

    if (lua_pcall(L, 1, 1, 0) != 0) {
        lua_pop(L, 1); // pop UIParent
        return false;
    }

    scale = lua_tonumber(L, -1);
    lua_pop(L, 2); // pop scale and UIParent

    return true;
}

inline bool GetPoint(lua_State* L, int frame_idx, int point_index,
    std::string& point, std::string& relativeToName, std::string& relativePoint,
    double& xOfs, double& yOfs)
{
    lua_getfield(L, frame_idx, "GetPoint");  // push frame.GetPoint function
    if (lua_type(L, -1) != 6) {              // LUA_TFUNCTION
        lua_pop(L, 1);
        return false;
    }

    lua_pushvalue(L, frame_idx);             // push frame as self
    lua_pushnumber(L, point_index);          // push index n

    if (lua_pcall(L, 2, 5, 0) != 0) {
        const char* err = lua_tostringnew(L, -1);
        lua_pop(L, 1);
        return false;
    }


    if (lua_type(L, -5) == 4)
        point = lua_tostringnew(L, -5);
    else
        point.clear();

    relativeToName.clear();
    if (!lua_isnil(L, -4)) {
        lua_pushvalue(L, -4); // push relativeTo object
        lua_getfield(L, -1, "GetName"); // push relativeTo:GetName
        if (lua_type(L, -1) == 6) { // is function
            lua_pushvalue(L, -2); // push relativeTo as self
            if (lua_pcall(L, 1, 1, 0) == 0) {
                if (lua_type(L, -1) == 4) {
                    relativeToName = lua_tostringnew(L, -1);
                }
                lua_pop(L, 1); // pop returned name
            }
            else {
                lua_pop(L, 1); // error msg
            }
        }
        else {
            lua_pop(L, 1); // pop non-function
        }
        lua_pop(L, 1); // pop relativeTo object
    }

    // relativePoint
    if (lua_type(L, -3) == 4)
        relativePoint = lua_tostringnew(L, -3);
    else
        relativePoint.clear();

    // xOfs
    xOfs = lua_tonumber(L, -2);

    // yOfs
    yOfs = lua_tonumber(L, -1);

    lua_pop(L, 5); // pop all return values

    return true;
}

//todo: 0x00489FA0 void __thiscall CLayoutFrame::SetHeight(CLayoutFrame *this, float height)
inline bool SetHeight(lua_State* L, int frame_idx, double height)
{
    lua_getfield(L, frame_idx, "SetHeight");  // push frame.SetHeight function
    if (lua_type(L, -1) != LUA_TFUNCTION) {
        lua_pop(L, 1);
        return false;  // function not found
    }

    lua_pushvalue(L, frame_idx);     // push frame as self
    lua_pushnumber(L, height);       // push height

    if (lua_pcall(L, 2, 0, 0) != 0) {  // pcall with 2 arguments: self, height
        const char* err = lua_tostringnew(L, -1);
        // handle error, e.g. print or log
        lua_pop(L, 1);
        return false;
    }

    return true;
}

//todo: 00489F80 void __thiscall CLayoutFrame::SetWidth(CLayoutFrame *this, float width)
inline bool SetWidth(lua_State* L, int frame_idx, double height)
{
    lua_getfield(L, frame_idx, "SetWidth");  // push frame.SetHeight function
    if (lua_type(L, -1) != LUA_TFUNCTION) {
        lua_pop(L, 1);
        return false;  // function not found
    }

    lua_pushvalue(L, frame_idx);     // push frame as self
    lua_pushnumber(L, height);       // push height

    if (lua_pcall(L, 2, 0, 0) != 0) {  // pcall with 2 arguments: self, height
        const char* err = lua_tostringnew(L, -1);
        // handle error, e.g. print or log
        lua_pop(L, 1);
        return false;
    }

    return true;
}

inline bool ConfigureWorldFrame(lua_State* L, bool enabledVariable)
{
    double scale = 0;
    GetEffectiveScale(L, scale);

    // Get ScreenHeight
    lua_getglobal(L, "GetScreenHeight");
    if (lua_pcall(L, 0, 1, 0) != 0) {
        return false;
    }
    double rawHeight = lua_tonumber(L, -1);
    lua_pop(L, 1);

    // Get ScreenWidth
    lua_getglobal(L, "GetScreenWidth");
    if (lua_pcall(L, 0, 1, 0) != 0) {
        return false;
    }
    double rawWidth = lua_tonumber(L, -1);
    lua_pop(L, 1);

    // Apply effective scale
    double ScreenHeight = rawHeight * scale;
    double ScreenWidth = rawWidth * scale;

    // Get WorldFrame
    lua_getglobal(L, "WorldFrame");

    // WorldFrame:ClearAllPoints()
    lua_getfield(L, -1, "ClearAllPoints");
    lua_pushvalue(L, -2); // self
    lua_pcall(L, 1, 0, 0);

    // WorldFrame:SetWidth(ScreenWidth)
    lua_getfield(L, -1, "SetWidth");
    lua_pushvalue(L, -2);
    lua_pushnumber(L, ScreenWidth);
    lua_pcall(L, 2, 0, 0);

    // WorldFrame:SetHeight(ScreenHeight * 5) or ScreenHeight
    lua_getfield(L, -1, "SetHeight");
    lua_pushvalue(L, -2);
    lua_pushnumber(L, enabledVariable ? ScreenHeight * 5.0 : ScreenHeight);
    lua_pcall(L, 2, 0, 0);

    // WorldFrame:SetPoint("BOTTOM")
    lua_getfield(L, -1, "SetPoint");
    lua_pushvalue(L, -2);
    lua_pushstring(L, "BOTTOM");
    lua_pcall(L, 2, 0, 0);

    lua_pop(L, 1); // Pop WorldFrame

    return true;
}

//todo: 0048A260 void __thiscall CLayoutFrame::SetPoint(CLayoutFrame *this, ANCHORPOINT point, CLayoutFrame *relative, ANCHORPOINT relativePoint, float offsetX, float offsetY, int shouldResize)
inline bool SetPoint(
    lua_State* L,
    int frame_idx,
    const char* point,
    int relativeTo_idx,
    const char* relativePoint,
    double x, double y)
{
    lua_getfield(L, frame_idx, "SetPoint");  // push frame.SetPoint function
    if (lua_type(L, -1) != LUA_TFUNCTION) {
        lua_pop(L, 1);
        return false;  // function not found
    }

    lua_pushvalue(L, frame_idx);  // push frame as self
    lua_pushstring(L, point);

    if (relativeTo_idx != -2) {
        lua_pushvalue(L, relativeTo_idx);  // push relativeTo frame
    }
    else {
        lua_pushnil(L);  // push nil for relativeTo if none
    }

    lua_pushstring(L, relativePoint);
    lua_pushnumber(L, x);
    lua_pushnumber(L, y);

    if (lua_pcall(L, 6, 0, 0) != 1) {
        const char* err = lua_tostringnew(L, -1);
        // handle error, e.g. print or log
        lua_pop(L, 1);
        return false;
    }

    return true;
}

//todo: 0x0048EB00 void CSimpleFrame::SetClampRectInsets(CSimpleFrame *this, float xMin, float xMax, float yMax, float yMin)
inline bool SetClampRectInsets(lua_State* L, int frame_idx, double left, double right, double top, double bottom)
{
    lua_getfield(L, frame_idx, "SetClampRectInsets");
    if (lua_type(L, -1) != LUA_TFUNCTION) {
        lua_pop(L, 1);
        return false;
    }

    lua_pushvalue(L, frame_idx);
    lua_pushnumber(L, left);
    lua_pushnumber(L, right);
    lua_pushnumber(L, top);
    lua_pushnumber(L, bottom);

    if (lua_pcall(L, 5, 0, 0) != 0) {
        lua_pop(L, 1);
        return false;
    }

    return true;
}

//todo 0048A130 void __thiscall CLayoutFrame::SetClampedToScreen(CLayoutFrame *this, int clamped)
inline bool SetClampedToScreen(lua_State* L, int frame_idx, bool clamped)
{
    lua_getfield(L, frame_idx, "SetClampedToScreen");
    if (lua_type(L, -1) != LUA_TFUNCTION) {
        lua_pop(L, 1);
        return false;
    }

    lua_pushvalue(L, frame_idx);
    lua_pushnumber(L, clamped ? 1 : 0);

    if (lua_pcall(L, 2, 0, 0) != 0) {
        lua_pop(L, 1);
        return false;
    }

    return true;
}

//todo: 0x00489FF0 void __thiscall CLayoutFrame::GetSize(CLayoutFrame *this, float *width, float *height, int ignoreRect)
inline bool GetSize(lua_State* L, int frame_idx, double& width, double& height)
{
    lua_getfield(L, frame_idx, "GetSize");
    if (lua_type(L, -1) != LUA_TFUNCTION) {
        lua_pop(L, 1);
        return false;
    }

    lua_pushvalue(L, frame_idx);

    if (lua_pcall(L, 1, 2, 0) != 0) {
        lua_pop(L, 1);
        return false;
    }

    if (lua_type(L, -2) != LUA_TNUMBER || lua_type(L, -1) != LUA_TNUMBER) {
        lua_pop(L, 2);
        return false;
    }

    const char* widthStr = lua_tostringnew(L, -2);
    const char* heightStr = lua_tostringnew(L, -1);
    width = widthStr ? atof(widthStr) : 0.0;
    height = heightStr ? atof(heightStr) : 0.0;

    lua_pop(L, 2);  // pop width and height

    return true;
}

inline bool IsFriendlyByReaction(CGUnit_C* unit) {
    int reaction = ObjectMgr::GetCGUnitPlayer()->UnitReaction(unit);
    bool canAttack = ObjectMgr::GetCGUnitPlayer()->CanAttack(unit);

    if (reaction >= 5) {
        return true;
    }
    else if (reaction == 4 && canAttack) {
        return false;
    }
    else if (reaction == 4 && !canAttack) {
        return true;
    }
    return false;
}

inline Frame* getNameplateByGuid(guid_t guid)
{
    if (!guid)
        return nullptr;

    for (auto& [index, holder] : Nameplates)
    {
        Frame* f = holder.nameplate;
        if (!f) continue;
        if (f->guid == guid)
            return f;
    }

    return nullptr;
}
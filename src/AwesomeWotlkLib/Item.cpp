#include "Item.h"
#include "Hooks.h"
#include "GameClient.h"


static uint32_t extractItemId(const char* hyperlink) {
    const char* itemPos = strstr(hyperlink, "|Hitem:");
    if (!itemPos)
        itemPos = strstr(hyperlink, "|hitem:");
    if (!itemPos)
        return 0;

    itemPos += 7; // Skip past "|Hitem:"
    char* endPtr;
    uint32_t itemId = strtoul(itemPos, &endPtr, 10);

    return (endPtr != itemPos && (*endPtr == ':' || *endPtr == '|')) ? itemId : 0;
}

static uintptr_t classTablePtr = GetDbcTable(0x00000147);
static const char* GetItemClassName(uint32_t classId) {
    auto* header = reinterpret_cast<DBCHeader*>(classTablePtr - 0x18);
    uint8_t rowBuffer[680];

    if (GetLocalizedRow(header, classId, rowBuffer)) {
        auto* itemClass = reinterpret_cast<ItemClassRec*>(rowBuffer);
        return reinterpret_cast<const char*>(itemClass->m_className_lang);
    }

    return "";
}

static uintptr_t subClassTablePtr = GetDbcTable(0x00000152);
static const char* GetItemSubClassName(uint32_t classId, uint32_t subClassId) {
    auto* header = reinterpret_cast<DBCHeader*>(subClassTablePtr - 0x18);
    uint8_t rowBuffer[680];

    for (uint32_t i = 0; i <= header->MaxIndex; ++i) {
        if (GetLocalizedRow(header, i, rowBuffer)) {
            auto* subclass = reinterpret_cast<ItemSubClassRec*>(rowBuffer);
            if (subclass->m_classID == classId && subclass->m_subClassID == subClassId)
                return reinterpret_cast<const char*>(subclass->m_displayName_lang);
        }
    }

    return "";
}

uintptr_t displayInfoTablePtr = GetDbcTable(0x00000149);
static const char* GetItemIcon(uint32_t displayId) {
    auto* header = reinterpret_cast<DBCHeader*>(displayInfoTablePtr - 0x18);
    uint8_t rowBuffer[680];

    if (GetLocalizedRow(header, displayId, rowBuffer)) {
        auto* itemData = reinterpret_cast<ItemDisplayInfoRec*>(rowBuffer);
        const char* iconName = reinterpret_cast<const char*>(itemData->m_inventoryIcon);

        if (iconName && iconName[0] != '\0') {
            static char fullPath[256];
            snprintf(fullPath, sizeof(fullPath), "Interface\\Icons\\%s", iconName);
            return fullPath;
        }
    }

    return "";
}

static int lua_GetItemInfoInstant(lua_State* L) {
    uintptr_t recordPtr = 0;
    uint32_t itemId = 0;

    if (lua_isnumber(L, 1)) {
        itemId = static_cast<uint32_t>(luaL_checknumber(L, 1));
    } else if (lua_isstring(L, 1)) {
        const char* input = luaL_checkstring(L, 1);
        itemId = getItemIDByName(input);
        if (!itemId)
            itemId = extractItemId(input);
    }

    if (itemId)
        recordPtr = getItemInfoById(itemCachePtr, itemId, 0, 0, 0, 0);

    if (!recordPtr)
        return 0;

    auto* item = reinterpret_cast<ItemCacheRec*>(recordPtr);

    lua_pushnumber(L, itemId);
    lua_pushstring(L, GetItemClassName(item->ClassId));
    lua_pushstring(L, GetItemSubClassName(item->ClassId, item->SubClassId));
    lua_pushstring(L, idToStr[item->InventoryType]);
    lua_pushstring(L, GetItemIcon(item->DisplayInfoId));
    lua_pushnumber(L, item->ClassId);
    lua_pushnumber(L, item->SubClassId);

    return 7;
}

static int lua_openmisclib(lua_State* L)
{
    lua_pushcfunction(L, lua_GetItemInfoInstant);
    lua_setglobal(L, "GetItemInfoInstant");
    return 0;
}

void Item::initialize()
{
    Hooks::FrameXML::registerLuaLib(lua_openmisclib);
}
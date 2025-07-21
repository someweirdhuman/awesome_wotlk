#include "Item.h"
#include "Hooks.h"
#include "GameClient.h"


static uint32_t extractItemId(const char* hyperlink) {
    const char* itemPos = strstr(hyperlink, "|Hitem:");
    if (!itemPos)
        itemPos = strstr(hyperlink, "|hitem:");

    if (itemPos) {
        itemPos += 7;
        char* endPtr;
        uint32_t itemId = strtoul(itemPos, &endPtr, 10);
        if (endPtr != itemPos && (*endPtr == ':' || *endPtr == '|'))
            return itemId;
    }
    return 0;
}

static uintptr_t classTablePtr = GetDbcTable(0x00000147);
static const char* GetItemClassName(uint32_t classId) {
    uint8_t rowBufferC[680];
    if (GetLocalizedRow((void*)(classTablePtr - 0x18), classId, rowBufferC)) {
        ItemClassRec* itemClass = (ItemClassRec*)rowBufferC;
        return (const char*)itemClass->m_className_lang;
    }
    return "";
}

static uintptr_t subClassTablePtr = GetDbcTable(0x00000152);
static const char* GetItemSubClassName(uint32_t classId, uint32_t subClassId) {
    DBCHeader* header = (DBCHeader*)(subClassTablePtr - 0x18);
    for (uint32_t i = 0; i <= header->MaxIndex; i++) {
        uint8_t rowBufferCS[680];
        if (GetLocalizedRow((void*)(subClassTablePtr - 0x18), i, rowBufferCS)) {
            ItemSubClassRec* subclass = (ItemSubClassRec*)rowBufferCS;
            if (subclass->m_classID == classId && subclass->m_subClassID == subClassId)
                return (const char*)subclass->m_displayName_lang;
        }
    }
    return "";
}

uintptr_t displayInfoTablePtr = GetDbcTable(0x00000149);
static const char* GetItemIcon(uint32_t displayId) {
    uint8_t rowBufferI[680];
    if (GetLocalizedRow((void*)(displayInfoTablePtr - 0x18), displayId, rowBufferI)) {
        ItemDisplayInfoRec* itemData = (ItemDisplayInfoRec*)rowBufferI;

        if (itemData->m_inventoryIcon != 0 || itemData->m_ID == displayId) {
            const char* iconName = (const char*)(itemData->m_inventoryIcon);
            if (!iconName || iconName[0] == '\0')
                return "";
            static char fullPath[256];
            sprintf(fullPath, "Interface\\Icons\\%s", iconName);
            return fullPath;
        }
    }
    return "";
}

static int lua_GetItemInfoInstant(lua_State* L) {
    uintptr_t recordPtr = 0;
    uint32_t itemId;

    if (lua_isnumber(L, 1)) {
        itemId = luaL_checknumber(L, 1);
        recordPtr = getItemInfoById(itemCachePtr, itemId, 0, 0, 0, 0);
    } else if (lua_isstring(L, 1)) {
        const char* str = luaL_checkstring(L, 1);
        itemId = getItemIDByName(str);
        if (!itemId)
            itemId = extractItemId(str);
        if (itemId)
            recordPtr = getItemInfoById(itemCachePtr, itemId, 0, 0, 0, 0);
    }

    if (!recordPtr)
        return 0;

    ItemCacheRec* item = (ItemCacheRec*)recordPtr;
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
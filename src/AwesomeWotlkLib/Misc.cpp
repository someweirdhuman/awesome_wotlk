#include "Misc.h"
#include "GameClient.h"
#include "Hooks.h"
#include "Utils.h"
#include <Windows.h>
#include <Detours/detours.h>
#define M_PI           3.14159265358979323846
#undef min
#undef max

static Console::CVar* s_cvar_cameraFov = NULL;


static int lua_FlashWindow(lua_State* L)
{
    HWND hwnd = GetGameWindow();
    if (hwnd) FlashWindow(hwnd, FALSE);
    return 0;
}

static int lua_IsWindowFocused(lua_State* L)
{
    HWND hwnd = GetGameWindow();
    if (!hwnd || GetForegroundWindow() != hwnd)
        return 0;
    lua_pushnumber(L, 1.f);
    return 1;
}

static int lua_FocusWindow(lua_State* L)
{
    HWND hwnd = GetGameWindow();
    if (hwnd) SetForegroundWindow(hwnd);
    return 0;
}

static int lua_CopyToClipboard(lua_State* L)
{
    const char* str = luaL_checkstring(L, 1);
    if (str && str[0]) CopyToClipboardU8(str, NULL);
    return 0;
}

static guid_t s_requestedInteraction = 0;
void ProcessQueuedInteraction()
{
    if (!s_requestedInteraction)
        return;

    CGObject_C* object = ObjectMgr::GetObjectPtr(s_requestedInteraction, TYPEMASK_GAMEOBJECT | TYPEMASK_UNIT);
    if (object) {
        object->OnRightClick(); // safe internal call, no Lua taint
    }

    s_requestedInteraction = 0;
}

bool IsGoodObject(uint8_t gameObjectType)
{
    switch (gameObjectType)
    {
        case GAMEOBJECT_TYPE_DOOR:
        case GAMEOBJECT_TYPE_BUTTON:
        case GAMEOBJECT_TYPE_QUESTGIVER:
        case GAMEOBJECT_TYPE_CHEST:
        case GAMEOBJECT_TYPE_BINDER:
        case GAMEOBJECT_TYPE_TRAP:
        case GAMEOBJECT_TYPE_CHAIR:
        case GAMEOBJECT_TYPE_SPELL_FOCUS:
        case GAMEOBJECT_TYPE_GOOBER:
        case GAMEOBJECT_TYPE_FISHINGNODE:
        case GAMEOBJECT_TYPE_SUMMONING_RITUAL:
        case GAMEOBJECT_TYPE_MAILBOX:
        case GAMEOBJECT_TYPE_MEETINGSTONE:
        case GAMEOBJECT_TYPE_FLAGSTAND:
        case GAMEOBJECT_TYPE_FLAGDROP:
        case GAMEOBJECT_TYPE_BARBER_CHAIR:
        case GAMEOBJECT_TYPE_GUILD_BANK:
        case GAMEOBJECT_TYPE_TRAPDOOR:
            return true;  
        default:
            return false; 
    }
}

static int lua_QueueInteract(lua_State* L)
{
    if (!IsInWorld())
        return 0;

    Player* player = ObjectMgr::GetPlayer();
    VecXYZ posPlayer;
    if (!player) return 0;

    player->ToUnit()->vmt->GetPosition(player->ToUnit(), &posPlayer);
    guid_t player_guid = player->entry->guid;

    guid_t candidate = 0;
    float bestDistance = 3000.0f;

    ObjectMgr::EnumObjects([&](guid_t guid) {
        if (guid == player_guid) return true;

        CGObject_C* object = ObjectMgr::GetObjectPtr(guid, TYPEMASK_GAMEOBJECT | TYPEMASK_UNIT);
        if (!object) return true;

        C3Vector objPos;
        object->GetPosition(objPos);

        VecXYZ testVec{ objPos.X, objPos.Y, objPos.Z };
        float distance = posPlayer.distance(testVec);

        if (distance > 20.0f || distance == 0.f || distance > bestDistance) {
            return true;
        }

        if (object->GetTypeID() == TYPEID_UNIT) {
            uint32_t dynFlags = object->GetValue<uint32_t>(UNIT_DYNAMIC_FLAGS);
            uint32_t unitFlags = object->GetValue<uint32_t>(UNIT_FIELD_FLAGS);
            uint32_t npcFlags = object->GetValue<uint32_t>(UNIT_NPC_FLAGS);

            bool isLootable = (dynFlags & UNIT_DYNFLAG_LOOTABLE) != 0;
            bool isSkinnable = (unitFlags & UNIT_FLAG_SKINNABLE) != 0;

            if ((!isLootable && !isSkinnable) && npcFlags == 0) {
                return true;
            }
        }
        else if (object->GetTypeID() == TYPEID_GAMEOBJECT) {
            uint32_t bytes = object->GetValue<uint32_t>(GAMEOBJECT_BYTES_1);
            uint8_t gameObjectType = (bytes >> 8) & 0xFF;
            if (!IsGoodObject(gameObjectType)) {
                return true;
            }
        }

        candidate = guid;
        bestDistance = distance;

        return true;
        });

    if (candidate != 0)
        s_requestedInteraction = candidate;

    return 0;
}

static int lua_openmisclib(lua_State* L)
{
    luaL_Reg funcs[] = {
        { "FlashWindow", lua_FlashWindow },
        { "IsWindowFocused", lua_IsWindowFocused },
        { "FocusWindow", lua_FocusWindow },
        { "CopyToClipboard", lua_CopyToClipboard },
        { "QueueInteract", lua_QueueInteract, },
    };

    for (const auto& [name, func] : funcs) {
        lua_pushcfunction(L, func);
        lua_setglobal(L, name);
    }

    Hooks::FrameScript::registerOnUpdate(ProcessQueuedInteraction);

    return 0;
}

static double parseFov(const char* v) { return  M_PI / 200.f * double(std::max(std::min(gc_atoi(&v), 200), 1)); }

static int CVarHandler_cameraFov(Console::CVar* cvar, const char* prevV, const char* newV, void* udata)
{
    if (Camera* camera = GetActiveCamera()) camera->fovInRadians = parseFov(newV);
    return 1;
}

static void(__fastcall* Camera_Initialize_orig)(Camera* self, void* edx, float a2, float a3, float fov) = (decltype(Camera_Initialize_orig))0x00607C20;
static void __fastcall Camera_Initialize_hk(Camera* self, void* edx, float a2, float a3, float fov)
{
    fov = parseFov(s_cvar_cameraFov->vStr);
    Camera_Initialize_orig(self, edx, a2, a3, fov);
}

void Misc::initialize()
{
    Hooks::FrameXML::registerCVar(&s_cvar_cameraFov, "cameraFov", NULL, (Console::CVarFlags)1, "100", CVarHandler_cameraFov);
    Hooks::FrameXML::registerLuaLib(lua_openmisclib);

    DetourAttach(&(LPVOID&)Camera_Initialize_orig, Camera_Initialize_hk);
}
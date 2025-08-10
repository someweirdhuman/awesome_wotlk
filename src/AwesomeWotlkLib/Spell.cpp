#include "Spell.h"
#include "Hooks.h"
#include "GameClient.h"
#include <Detours/detours.h>

static Console::CVar* s_cvar_enableStancePatch;
static int CVarHandler_enableStancePatch(Console::CVar*, const char*, const char* value, LPVOID) { return 1; }

uintptr_t spellTablePtr = GetDbcTable(0x00000194);
static int lua_GetSpellBaseCooldown(lua_State* L) {
    uint8_t rowBuffer[680];
    uint32_t spellId = luaL_checknumber(L, 1);

    if (!GetLocalizedRow((void*)(spellTablePtr - 0x18), spellId, rowBuffer))
        return 0;

    SpellRec* spell = (SpellRec*)rowBuffer;
    uint32_t cdTime = spell->RecoveryTime ? spell->RecoveryTime : spell->CategoryRecoveryTime;
    uint32_t gcdTime = spell->StartRecoveryTime;

    if (cdTime == 0) {
        for (int i = 0; i < 3; i++) {
            if (spell->Effect[i] == 0)
                continue;

            uint32_t triggeredSpellId = spell->EffectTriggerSpell[i];
            if (triggeredSpellId == 0 || triggeredSpellId == spellId)
                continue;

            uint8_t rowBufferTrig[680];
            if (!GetLocalizedRow((void*)(spellTablePtr - 0x18), triggeredSpellId, rowBufferTrig))
                continue;

            SpellRec* spellTrig = (SpellRec*)rowBufferTrig;
            uint32_t trigCd = spellTrig->RecoveryTime ? spellTrig->RecoveryTime : spellTrig->CategoryRecoveryTime;
            uint32_t trigGcd = spellTrig->StartRecoveryTime;

            if (trigCd > cdTime)
                cdTime = trigCd;
            if (trigGcd > gcdTime)
                gcdTime = trigGcd;
        }
    }
    lua_pushnumber(L, cdTime);
    lua_pushnumber(L, gcdTime);
    return 2;
}

typedef int(__cdecl* SpellCastFn)(int a1, int a2, int a3, int a4, int a5);
static SpellCastFn Spell_OnCastOriginal = (SpellCastFn)0x0080DA40;
int __cdecl Spell_OnCastHook(int spellId, int a2, int a3, int a4, int a5)
{
    bool success = Spell_OnCastOriginal(spellId, a2, a3, a4, a5);
    if (success && Spell::IsForm(spellId) && std::atoi(s_cvar_enableStancePatch->vStr) == 1)
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

    return success;
}

static int lua_openmisclib(lua_State* L)
{
    lua_pushcfunction(L, lua_GetSpellBaseCooldown);
    lua_setglobal(L, "GetSpellBaseCooldown");
    return 0;
}

void Spell::initialize()
{
    Hooks::FrameXML::registerLuaLib(lua_openmisclib);
    Hooks::FrameXML::registerCVar(&s_cvar_enableStancePatch, "enableStancePatch", NULL, (Console::CVarFlags)1, "0", CVarHandler_enableStancePatch);

    DetourAttach(&(LPVOID&)Spell_OnCastOriginal, Spell_OnCastHook);
}
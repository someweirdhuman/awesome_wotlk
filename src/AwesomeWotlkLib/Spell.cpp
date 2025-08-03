#include "Spell.h"
#include "Hooks.h"
#include "GameClient.h"
#include <Detours/detours.h>
#include <string>

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

static int lua_openspelllib(lua_State* L)
{
    lua_pushcfunction(L, lua_GetSpellBaseCooldown);
    lua_setglobal(L, "GetSpellBaseCooldown");
    return 0;
}

void Spell::initialize()
{
    Hooks::FrameXML::registerLuaLib(lua_openspelllib);
}
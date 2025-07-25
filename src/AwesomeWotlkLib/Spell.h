#pragma once
#include <array>
#include <optional>

inline constexpr uint8_t OFFSET_SHAPESHIFT_FORM = 3;

namespace Spell {
    enum ShapeshiftForm : uint8_t
    {
        FORM_NONE = 0x00,
        FORM_CAT = 0x01,
        FORM_TREE = 0x02,
        FORM_TRAVEL = 0x03,
        FORM_AQUA = 0x04,
        FORM_BEAR = 0x05,
        FORM_AMBIENT = 0x06,
        FORM_GHOUL = 0x07,
        FORM_DIREBEAR = 0x08,
        FORM_STEVES_GHOUL = 0x09,
        FORM_THARONJA_SKELETON = 0x0A,
        FORM_TEST_OF_STRENGTH = 0x0B,
        FORM_BLB_PLAYER = 0x0C,
        FORM_SHADOW_DANCE = 0x0D,
        FORM_CREATUREBEAR = 0x0E,
        FORM_CREATURECAT = 0x0F,
        FORM_GHOSTWOLF = 0x10,
        FORM_BATTLESTANCE = 0x11,
        FORM_DEFENSIVESTANCE = 0x12,
        FORM_BERSERKERSTANCE = 0x13,
        FORM_TEST = 0x14,
        FORM_ZOMBIE = 0x15,
        FORM_METAMORPHOSIS = 0x16,
        FORM_UNDEAD = 0x19,
        FORM_MASTER_ANGLER = 0x1A,
        FORM_FLIGHT_EPIC = 0x1B,
        FORM_SHADOW = 0x1C,
        FORM_FLIGHT = 0x1D,
        FORM_STEALTH = 0x1E,
        FORM_MOONKIN = 0x1F,
        FORM_SPIRITOFREDEMPTION = 0x20
    };

    constexpr std::array<std::pair<ShapeshiftForm, uint32_t>, 9> ShapeshiftFormSpells = {
        {
            { FORM_CAT, 768 },
            { FORM_TRAVEL, 783 },
            { FORM_AQUA, 1066 },
            { FORM_BEAR, 5487 },
            { FORM_DIREBEAR, 9634 },
            { FORM_TREE, 33891 },
            { FORM_BATTLESTANCE, 2457 },
            { FORM_DEFENSIVESTANCE, 71 },
            { FORM_BERSERKERSTANCE, 2458 },
        }
    };

    inline std::optional<uint32_t> GetSpellId(ShapeshiftForm form)
    {
        for (const auto& [f, spell] : ShapeshiftFormSpells)
            if (f == form)
                return spell;
        return std::nullopt;
    }

    inline std::optional<ShapeshiftForm> GetFormFromSpell(uint32_t spellId)
    {
        for (const auto& [f, spell] : ShapeshiftFormSpells)
            if (spell == spellId)
                return f;
        return std::nullopt;
    }

    inline bool IsForm(uint32_t spellId)
    {
        return GetFormFromSpell(spellId).has_value();
    }
    void initialize();
}
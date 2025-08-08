#pragma once
#include <Windows.h>
#include <cstdint>
#include <cstdarg>
#include <functional>
#include <string>
#include <algorithm>

/*
    Game client types/functions/bindings and other import
*/

// Types
struct lua_State;
struct WorldFrame;
struct Camera;
struct Status;
struct Frame;
struct XMLObject;
struct Object;
struct ObjectVtbl;
struct ObjectEntry;
struct Unit;
struct UnitVtbl;
struct UnitEntry;
struct Player;
struct PlayerVtbl;
struct PlayerEntry;
using guid_t = uint64_t;
using lua_Number = double;

template <typename T> struct Vec2D { T x, y; };
template <typename T> struct Vec3D { T x, y, z; };
template <typename T> struct Vec4D { T x, y, z, o; };
struct VecXYZ : Vec3D<float> {

    inline VecXYZ operator-(const VecXYZ& r) { return { x - r.x, y - r.y, z - r.z }; }

    inline float distance(const VecXYZ& other)
    {
        VecXYZ diff = (*this) - other;
        return std::sqrtf(std::powf(diff.x, 2) + std::powf(diff.y, 2) + std::powf(diff.z, 2));
    }
};

struct TerrainClickEvent {
    uint64_t GUID;
    float x, y, z;
    uint32_t button;
};
const char* idToStr[35] = {
    "INVTYPE_NON_EQUIP",              //  0
    "INVTYPE_HEAD",                   //  1
    "INVTYPE_NECK",                   //  2
    "INVTYPE_SHOULDER",               //  3
    "INVTYPE_BODY",                   //  4
    "INVTYPE_CHEST",                  //  5
    "INVTYPE_WAIST",                  //  6
    "INVTYPE_LEGS",                   //  7
    "INVTYPE_FEET",                   //  8
    "INVTYPE_WRIST",                  //  9
    "INVTYPE_HAND",                   // 10
    "INVTYPE_FINGER",                 // 11
    "INVTYPE_TRINKET",                // 12
    "INVTYPE_WEAPON",                 // 13
    "INVTYPE_SHIELD",                 // 14
    "INVTYPE_RANGED",                 // 15
    "INVTYPE_CLOAK",                  // 16
    "INVTYPE_2HWEAPON",               // 17
    "INVTYPE_BAG",                    // 18
    "INVTYPE_TABARD",                 // 19
    "INVTYPE_ROBE",                   // 20
    "INVTYPE_WEAPONMAINHAND",         // 21
    "INVTYPE_WEAPONOFFHAND",          // 22
    "INVTYPE_HOLDABLE",               // 23
    "INVTYPE_AMMO",                   // 24
    "INVTYPE_THROWN",                 // 25
    "INVTYPE_RANGEDRIGHT",            // 26
    "INVTYPE_QUIVER",                 // 27
    "INVTYPE_RELIC",                  // 28
    "INVTYPE_PROFESSION_TOOL",        // 29
    "INVTYPE_PROFESSION_GEAR",        // 30
    "INVTYPE_EQUIPABLESPELL_OFFENSIVE", // 31
    "INVTYPE_EQUIPABLESPELL_UTILITY",   // 32
    "INVTYPE_EQUIPABLESPELL_DEFENSIVE", // 33
    "INVTYPE_EQUIPABLESPELL_WEAPON"     // 34
};

struct DBCHeader {
    uint32_t junk[3];
    uint32_t MaxIndex;
    uint32_t MinIndex;
};

struct ItemCacheRec {
    uint32_t ID;
    uint32_t ClassId;
    uint32_t SubClassId;
    int32_t Unk0;
    uint32_t DisplayInfoId;
    uint32_t gap0[5]; // ItemQuality Quality; ItemFlags TypeFlags; int32_t BuyPrice; int32_t Faction; int32_t SellPrice;
    uint32_t InventoryType;
    //...
};

struct ItemClassRec {
    uint32_t m_classID;
    uint32_t m_subclassMapID;
    uint32_t m_flags;
    uint32_t m_className_lang;
};

struct ItemSubClassRec {
    uint32_t m_classID;
    uint32_t m_subClassID;
    uint32_t m_prerequisiteProficiency;
    uint32_t m_postrequisiteProficiency;
    uint32_t m_flags;
    uint32_t m_displayFlags;
    uint32_t m_weaponParrySeq;
    uint32_t m_weaponReadySeq;
    uint32_t m_weaponAttackSeq;
    uint32_t m_WeaponSwingSize;
    uint32_t m_displayName_lang;
    //uint32_t m_verboseName_lang;
};

struct ItemDisplayInfoRec {
    uint32_t m_ID;
    uint32_t m_modelName[2];
    uint32_t m_modelTexture[2];
    uint32_t m_inventoryIcon;
    //uint32_t m_groundModel;
    //uint32_t m_geosetGroup[3];
    //uint32_t m_spellVisualID;
    //uint32_t m_groupSoundIndex;
    //uint32_t m_helmetGeosetVisID[2];
    //uint32_t m_texture[8];
    //uint32_t m_itemVisual;
};

typedef uint32_t(__cdecl* GetItemIDByName_t)(const char* name);
static GetItemIDByName_t getItemIDByName = (GetItemIDByName_t)0x00709DE0;

typedef uintptr_t(__thiscall* GetItemInfoBlockByIdDelegate)(void* instance, uint32_t id, uint64_t* guid, int a4, int a5, int a6);
static auto getItemInfoById = (GetItemInfoBlockByIdDelegate)(0x0067CA30);
static void* itemCachePtr = (void*)(0x00C5D828);

struct Flag96 {
    uint32_t part1;
    uint32_t part2;
    uint32_t part3;
};

struct SpellRec {
    uint32_t Id;
    uint32_t Category;
    uint32_t Dispel;
    int32_t Mechanic;

    uint32_t Attributes;
    uint32_t AttributesEx;
    uint32_t AttributesEx2;
    uint32_t AttributesEx3;
    uint32_t AttributesEx4;
    uint32_t AttributesEx5;
    uint32_t AttributesEx6;
    uint32_t AttributesEx7;

    uint32_t Stances;
    uint32_t unk_320_2;
    uint32_t StancesNot;
    uint32_t unk_320_3;
    uint32_t Targets;
    uint32_t TargetCreatureType;
    uint32_t RequiresSpellFocus;
    uint32_t FacingCasterFlags;
    uint32_t CasterAuraState;
    uint32_t TargetAuraState;
    uint32_t CasterAuraStateNot;
    uint32_t TargetAuraStateNot;
    uint32_t casterAuraSpell;
    uint32_t targetAuraSpell;
    uint32_t excludeCasterAuraSpell;
    uint32_t excludeTargetAuraSpell;
    uint32_t CastingTimeIndex;
    uint32_t RecoveryTime;
    uint32_t CategoryRecoveryTime;
    uint32_t InterruptFlags;
    uint32_t AuraInterruptFlags;
    uint32_t ChannelInterruptFlags;
    uint32_t procFlags;
    uint32_t procChance;
    uint32_t procCharges;
    uint32_t maxLevel;
    uint32_t baseLevel;
    uint32_t spellLevel;
    uint32_t DurationIndex;
    int32_t powerType;
    uint32_t manaCost;
    uint32_t manaCostPerlevel;
    uint32_t manaPerSecond;
    uint32_t manaPerSecondPerLevel;
    uint32_t rangeIndex;
    float speed;
    uint32_t modalNextSpell;
    uint32_t StackAmount;
    uint32_t Totem[2];
    int32_t Reagent[8];
    uint32_t ReagentCount[8];
    int32_t EquippedItemClass;
    int32_t EquippedItemSubClassMask;
    int32_t EquippedItemInventoryTypeMask;

    int32_t Effect[3];
    int32_t EffectDieSides[3];
    //int32_t EffectBaseDice[3];
    //float EffectDicePerLevel[3];
    float EffectRealPointsPerLevel[3];
    int32_t EffectBasePoints[3];
    uint32_t EffectMechanic[3];
    uint32_t EffectImplicitTargetA[3];
    uint32_t EffectImplicitTargetB[3];
    uint32_t EffectRadiusIndex[3];
    uint32_t EffectApplyAuraName[3];
    uint32_t EffectAmplitude[3];
    float EffectMultipleValue[3];
    uint32_t EffectChainTarget[3];
    uint32_t EffectItemType[3];
    int32_t EffectMiscValue[3];
    int32_t EffectMiscValueB[3];
    uint32_t EffectTriggerSpell[3];
    float EffectPointsPerComboPoint[3];
    Flag96 EffectSpellClassMask[3];

    uint32_t SpellVisual[2];
    uint32_t SpellIconID;
    uint32_t activeIconID;
    uint32_t spellPriority;

    uint32_t SpellNameOffset;  // string block
    uint32_t RankOffset;
    uint32_t DescriptionOffset;
    uint32_t ToolTipOffset;

    uint32_t ManaCostPercentage;
    uint32_t StartRecoveryCategory;
    uint32_t StartRecoveryTime;
    uint32_t MaxTargetLevel;
    uint32_t SpellFamilyName;
    Flag96 SpellFamilyFlags;
    uint32_t MaxAffectedTargets;
    uint32_t DmgClass;
    uint32_t PreventionType;
    uint32_t StanceBarOrder;

    float DmgMultiplier[3];
    uint32_t MinFactionId;
    uint32_t MinReputation;
    uint32_t RequiredAuraVision;
    uint32_t TotemCategory[2];
    int32_t AreaGroupId;
    int32_t SchoolMask;
    uint32_t runeCostID;
    uint32_t spellMissileID;
    uint32_t PowerDisplayId;
    float unk_320_4[3];
    uint32_t spellDescriptionVariableID;
    uint32_t SpellDifficultyId;
};
static_assert(sizeof(SpellRec) == 0x2A8);

enum UIFrame {
    CurrentFrame_Ptr = 0x00B499A8,
    CurrentFrame_Offset = 0x78,
    UIBase = 0x00B499A8,
    FirstFrame = 0x0CD4,
    NextFrame = 0x0CCC,
    UnkDivWidth = 0x00AC0CB4,
    UnkDivHeight = 0x00AC0CB8,
    ScreenWidth = 0x00C7D2C8,
    ScreenHeight = 0x00C7D2C4,
    FrameLeft = 0x68,
    FrameRight = 0x70,
    FrameTop = 0x6C,
    FrameBottom = 0x64,
    ParentPtr = 0x94,
    EffectiveScale = 0x7C,
    Name = 0x1C,
    Visible = 0xE0,
};

enum UnitFlags : uint32_t {
    UNIT_FLAG_SERVER_CONTROLLED = 0x00000001,           // set only when unit movement is controlled by server - by SPLINE/MONSTER_MOVE packets, together with UNIT_FLAG_STUNNED; only set to units controlled by client; client function CGUnit_C::IsClientControlled returns false when set for owner
    UNIT_FLAG_NON_ATTACKABLE = 0x00000002,           // not attackable, set when creature starts to cast spells with SPELL_EFFECT_SPAWN and cast time, removed when spell hits caster, original name is UNIT_FLAG_SPAWNING. Rename when it will be removed from all scripts
    UNIT_FLAG_REMOVE_CLIENT_CONTROL = 0x00000004,           // This is a legacy flag used to disable movement player's movement while controlling other units, SMSG_CLIENT_CONTROL replaces this functionality clientside now. CONFUSED and FLEEING flags have the same effect on client movement asDISABLE_MOVE_CONTROL in addition to preventing spell casts/autoattack (they all allow climbing steeper hills and emotes while moving)
    UNIT_FLAG_PLAYER_CONTROLLED = 0x00000008,           // controlled by player, use _IMMUNE_TO_PC instead of _IMMUNE_TO_NPC
    UNIT_FLAG_RENAME = 0x00000010,
    UNIT_FLAG_PREPARATION = 0x00000020,           // don't take reagents for spells with SPELL_ATTR5_NO_REAGENT_WHILE_PREP
    UNIT_FLAG_UNK_6 = 0x00000040,
    UNIT_FLAG_NOT_ATTACKABLE_1 = 0x00000080,           // ?? (UNIT_FLAG_PLAYER_CONTROLLED | UNIT_FLAG_NOT_ATTACKABLE_1) is NON_PVP_ATTACKABLE
    UNIT_FLAG_IMMUNE_TO_PC = 0x00000100,           // disables combat/assistance with PlayerCharacters (PC) - see Unit::IsValidAttackTarget, Unit::IsValidAssistTarget
    UNIT_FLAG_IMMUNE_TO_NPC = 0x00000200,           // disables combat/assistance with NonPlayerCharacters (NPC) - see Unit::IsValidAttackTarget, Unit::IsValidAssistTarget
    UNIT_FLAG_LOOTING = 0x00000400,           // loot animation
    UNIT_FLAG_PET_IN_COMBAT = 0x00000800,           // on player pets: whether the pet is chasing a target to attack || on other units: whether any of the unit's minions is in combat
    UNIT_FLAG_PVP_ENABLING = 0x00001000,           // changed in 3.0.3, now UNIT_BYTES_2_OFFSET_PVP_FLAG from UNIT_FIELD_BYTES_2
    UNIT_FLAG_SILENCED = 0x00002000,           // silenced, 2.1.1
    UNIT_FLAG_CANT_SWIM = 0x00004000,           // TITLE Can't Swim
    UNIT_FLAG_CAN_SWIM = 0x00008000,           // TITLE Can Swim DESCRIPTION shows swim animation in water
    UNIT_FLAG_NON_ATTACKABLE_2 = 0x00010000,           // removes attackable icon, if on yourself, cannot assist self but can cast TARGET_SELF spells - added by SPELL_AURA_MOD_UNATTACKABLE
    UNIT_FLAG_PACIFIED = 0x00020000,           // 3.0.3 ok
    UNIT_FLAG_STUNNED = 0x00040000,           // 3.0.3 ok
    UNIT_FLAG_IN_COMBAT = 0x00080000,
    UNIT_FLAG_ON_TAXI = 0x00100000,           // disable casting at client side spell not allowed by taxi flight (mounted?), probably used with 0x4 flag
    UNIT_FLAG_DISARMED = 0x00200000,           // 3.0.3, disable melee spells casting..., "Required melee weapon" added to melee spells tooltip.
    UNIT_FLAG_CONFUSED = 0x00400000,
    UNIT_FLAG_FLEEING = 0x00800000,
    UNIT_FLAG_POSSESSED = 0x01000000,           // under direct client control by a player (possess or vehicle)
    UNIT_FLAG_UNINTERACTIBLE = 0x02000000,
    UNIT_FLAG_SKINNABLE = 0x04000000,
    UNIT_FLAG_MOUNT = 0x08000000,
    UNIT_FLAG_UNK_28 = 0x10000000,
    UNIT_FLAG_PREVENT_EMOTES_FROM_CHAT_TEXT = 0x20000000,   // Prevent automatically playing emotes from parsing chat text, for example "lol" in /say, ending message with ? or !, or using /yell
    UNIT_FLAG_SHEATHE = 0x40000000,
    UNIT_FLAG_IMMUNE = 0x80000000,           // Immune to damage

    UNIT_FLAG_DISALLOWED = (UNIT_FLAG_SERVER_CONTROLLED | UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_REMOVE_CLIENT_CONTROL |
    UNIT_FLAG_PLAYER_CONTROLLED | UNIT_FLAG_RENAME | UNIT_FLAG_PREPARATION | /* UNIT_FLAG_UNK_6 | */
        UNIT_FLAG_NOT_ATTACKABLE_1 | UNIT_FLAG_LOOTING | UNIT_FLAG_PET_IN_COMBAT | UNIT_FLAG_PVP_ENABLING |
        UNIT_FLAG_SILENCED | UNIT_FLAG_NON_ATTACKABLE_2 | UNIT_FLAG_PACIFIED | UNIT_FLAG_STUNNED |
        UNIT_FLAG_IN_COMBAT | UNIT_FLAG_ON_TAXI | UNIT_FLAG_DISARMED | UNIT_FLAG_CONFUSED | UNIT_FLAG_FLEEING |
        UNIT_FLAG_POSSESSED | UNIT_FLAG_SKINNABLE | UNIT_FLAG_MOUNT | UNIT_FLAG_UNK_28 |
        UNIT_FLAG_PREVENT_EMOTES_FROM_CHAT_TEXT | UNIT_FLAG_SHEATHE | UNIT_FLAG_IMMUNE), // SKIP

    UNIT_FLAG_ALLOWED = (0xFFFFFFFF & ~UNIT_FLAG_DISALLOWED) // SKIP
};

enum TypeMask : uint32_t {
    TYPEMASK_OBJECT = 0x1,
    TYPEMASK_ITEM = 0x2,
    TYPEMASK_CONTAINER = 0x4,
    TYPEMASK_UNIT = 0x8,
    TYPEMASK_PLAYER = 0x10,
    TYPEMASK_GAMEOBJECT = 0x20,
    TYPEMASK_DYNAMICOBJECT = 0x40,
    TYPEMASK_CORPSE = 0x80,
};

enum TypeID
{
    TYPEID_OBJECT = 0,
    TYPEID_ITEM = 1,
    TYPEID_CONTAINER = 2,
    TYPEID_UNIT = 3,
    TYPEID_PLAYER = 4,
    TYPEID_GAMEOBJECT = 5,
    TYPEID_DYNAMICOBJECT = 6,
    TYPEID_CORPSE = 7,
    NUM_TYPEIDS = 8
};

enum EObjectFields
{
    OBJECT_FIELD_GUID = 0x0000, // Size: 2, Type: LONG, Flags: PUBLIC
    OBJECT_FIELD_TYPE = 0x0002, // Size: 1, Type: INT, Flags: PUBLIC
    OBJECT_FIELD_ENTRY = 0x0003, // Size: 1, Type: INT, Flags: PUBLIC
    OBJECT_FIELD_SCALE_X = 0x0004, // Size: 1, Type: FLOAT, Flags: PUBLIC
    OBJECT_FIELD_PADDING = 0x0005, // Size: 1, Type: INT, Flags: NONE
    OBJECT_END = 0x0006,
};

enum EGameObjectFields
{
    OBJECT_FIELD_CREATED_BY = OBJECT_END + 0x0000, // Size: 2, Type: LONG, Flags: PUBLIC
    GAMEOBJECT_DISPLAYID = OBJECT_END + 0x0002, // Size: 1, Type: INT, Flags: PUBLIC
    GAMEOBJECT_FLAGS = OBJECT_END + 0x0003, // Size: 1, Type: INT, Flags: PUBLIC
    GAMEOBJECT_PARENTROTATION = OBJECT_END + 0x0004, // Size: 4, Type: FLOAT, Flags: PUBLIC
    GAMEOBJECT_DYNAMIC = OBJECT_END + 0x0008, // Size: 1, Type: TWO_SHORT, Flags: DYNAMIC
    GAMEOBJECT_FACTION = OBJECT_END + 0x0009, // Size: 1, Type: INT, Flags: PUBLIC
    GAMEOBJECT_LEVEL = OBJECT_END + 0x000A, // Size: 1, Type: INT, Flags: PUBLIC
    GAMEOBJECT_BYTES_1 = OBJECT_END + 0x000B, // Size: 1, Type: BYTES, Flags: PUBLIC
    GAMEOBJECT_END = OBJECT_END + 0x000C,
};

enum GameobjectTypes
{
    GAMEOBJECT_TYPE_DOOR = 0,
    GAMEOBJECT_TYPE_BUTTON = 1,
    GAMEOBJECT_TYPE_QUESTGIVER = 2,
    GAMEOBJECT_TYPE_CHEST = 3,
    GAMEOBJECT_TYPE_BINDER = 4,
    GAMEOBJECT_TYPE_GENERIC = 5,
    GAMEOBJECT_TYPE_TRAP = 6,
    GAMEOBJECT_TYPE_CHAIR = 7,
    GAMEOBJECT_TYPE_SPELL_FOCUS = 8,
    GAMEOBJECT_TYPE_TEXT = 9,
    GAMEOBJECT_TYPE_GOOBER = 10,
    GAMEOBJECT_TYPE_TRANSPORT = 11,
    GAMEOBJECT_TYPE_AREADAMAGE = 12,
    GAMEOBJECT_TYPE_CAMERA = 13,
    GAMEOBJECT_TYPE_MAP_OBJECT = 14,
    GAMEOBJECT_TYPE_MO_TRANSPORT = 15,
    GAMEOBJECT_TYPE_DUEL_ARBITER = 16,
    GAMEOBJECT_TYPE_FISHINGNODE = 17,
    GAMEOBJECT_TYPE_SUMMONING_RITUAL = 18,
    GAMEOBJECT_TYPE_MAILBOX = 19,
    GAMEOBJECT_TYPE_DO_NOT_USE = 20,
    GAMEOBJECT_TYPE_GUARDPOST = 21,
    GAMEOBJECT_TYPE_SPELLCASTER = 22,
    GAMEOBJECT_TYPE_MEETINGSTONE = 23,
    GAMEOBJECT_TYPE_FLAGSTAND = 24,
    GAMEOBJECT_TYPE_FISHINGHOLE = 25,
    GAMEOBJECT_TYPE_FLAGDROP = 26,
    GAMEOBJECT_TYPE_MINI_GAME = 27,
    GAMEOBJECT_TYPE_DO_NOT_USE_2 = 28,
    GAMEOBJECT_TYPE_CAPTURE_POINT = 29,
    GAMEOBJECT_TYPE_AURA_GENERATOR = 30,
    GAMEOBJECT_TYPE_DUNGEON_DIFFICULTY = 31,
    GAMEOBJECT_TYPE_BARBER_CHAIR = 32,
    GAMEOBJECT_TYPE_DESTRUCTIBLE_BUILDING = 33,
    GAMEOBJECT_TYPE_GUILD_BANK = 34,
    GAMEOBJECT_TYPE_TRAPDOOR = 35
};

enum EUnitFields
{
    UNIT_FIELD_CHARM = OBJECT_END + 0x0000, // Size: 2, Type: LONG, Flags: PUBLIC
    UNIT_FIELD_SUMMON = OBJECT_END + 0x0002, // Size: 2, Type: LONG, Flags: PUBLIC
    UNIT_FIELD_CRITTER = OBJECT_END + 0x0004, // Size: 2, Type: LONG, Flags: PRIVATE
    UNIT_FIELD_CHARMEDBY = OBJECT_END + 0x0006, // Size: 2, Type: LONG, Flags: PUBLIC
    UNIT_FIELD_SUMMONEDBY = OBJECT_END + 0x0008, // Size: 2, Type: LONG, Flags: PUBLIC
    UNIT_FIELD_CREATEDBY = OBJECT_END + 0x000A, // Size: 2, Type: LONG, Flags: PUBLIC
    UNIT_FIELD_TARGET = OBJECT_END + 0x000C, // Size: 2, Type: LONG, Flags: PUBLIC
    UNIT_FIELD_CHANNEL_OBJECT = OBJECT_END + 0x000E, // Size: 2, Type: LONG, Flags: PUBLIC
    UNIT_CHANNEL_SPELL = OBJECT_END + 0x0010, // Size: 1, Type: INT, Flags: PUBLIC
    UNIT_FIELD_BYTES_0 = OBJECT_END + 0x0011, // Size: 1, Type: BYTES, Flags: PUBLIC
    UNIT_FIELD_HEALTH = OBJECT_END + 0x0012, // Size: 1, Type: INT, Flags: PUBLIC
    UNIT_FIELD_POWER1 = OBJECT_END + 0x0013, // Size: 1, Type: INT, Flags: PUBLIC
    UNIT_FIELD_POWER2 = OBJECT_END + 0x0014, // Size: 1, Type: INT, Flags: PUBLIC
    UNIT_FIELD_POWER3 = OBJECT_END + 0x0015, // Size: 1, Type: INT, Flags: PUBLIC
    UNIT_FIELD_POWER4 = OBJECT_END + 0x0016, // Size: 1, Type: INT, Flags: PUBLIC
    UNIT_FIELD_POWER5 = OBJECT_END + 0x0017, // Size: 1, Type: INT, Flags: PUBLIC
    UNIT_FIELD_POWER6 = OBJECT_END + 0x0018, // Size: 1, Type: INT, Flags: PUBLIC
    UNIT_FIELD_POWER7 = OBJECT_END + 0x0019, // Size: 1, Type: INT, Flags: PUBLIC
    UNIT_FIELD_MAXHEALTH = OBJECT_END + 0x001A, // Size: 1, Type: INT, Flags: PUBLIC
    UNIT_FIELD_MAXPOWER1 = OBJECT_END + 0x001B, // Size: 1, Type: INT, Flags: PUBLIC
    UNIT_FIELD_MAXPOWER2 = OBJECT_END + 0x001C, // Size: 1, Type: INT, Flags: PUBLIC
    UNIT_FIELD_MAXPOWER3 = OBJECT_END + 0x001D, // Size: 1, Type: INT, Flags: PUBLIC
    UNIT_FIELD_MAXPOWER4 = OBJECT_END + 0x001E, // Size: 1, Type: INT, Flags: PUBLIC
    UNIT_FIELD_MAXPOWER5 = OBJECT_END + 0x001F, // Size: 1, Type: INT, Flags: PUBLIC
    UNIT_FIELD_MAXPOWER6 = OBJECT_END + 0x0020, // Size: 1, Type: INT, Flags: PUBLIC
    UNIT_FIELD_MAXPOWER7 = OBJECT_END + 0x0021, // Size: 1, Type: INT, Flags: PUBLIC
    UNIT_FIELD_POWER_REGEN_FLAT_MODIFIER = OBJECT_END + 0x0022, // Size: 7, Type: FLOAT, Flags: PRIVATE, OWNER
    UNIT_FIELD_POWER_REGEN_INTERRUPTED_FLAT_MODIFIER = OBJECT_END + 0x0029, // Size: 7, Type: FLOAT, Flags: PRIVATE, OWNER
    UNIT_FIELD_LEVEL = OBJECT_END + 0x0030, // Size: 1, Type: INT, Flags: PUBLIC
    UNIT_FIELD_FACTIONTEMPLATE = OBJECT_END + 0x0031, // Size: 1, Type: INT, Flags: PUBLIC
    UNIT_VIRTUAL_ITEM_SLOT_ID = OBJECT_END + 0x0032, // Size: 3, Type: INT, Flags: PUBLIC
    UNIT_FIELD_FLAGS = OBJECT_END + 0x0035, // Size: 1, Type: INT, Flags: PUBLIC
    UNIT_FIELD_FLAGS_2 = OBJECT_END + 0x0036, // Size: 1, Type: INT, Flags: PUBLIC
    UNIT_FIELD_AURASTATE = OBJECT_END + 0x0037, // Size: 1, Type: INT, Flags: PUBLIC
    UNIT_FIELD_BASEATTACKTIME = OBJECT_END + 0x0038, // Size: 2, Type: INT, Flags: PUBLIC
    UNIT_FIELD_RANGEDATTACKTIME = OBJECT_END + 0x003A, // Size: 1, Type: INT, Flags: PRIVATE
    UNIT_FIELD_BOUNDINGRADIUS = OBJECT_END + 0x003B, // Size: 1, Type: FLOAT, Flags: PUBLIC
    UNIT_FIELD_COMBATREACH = OBJECT_END + 0x003C, // Size: 1, Type: FLOAT, Flags: PUBLIC
    UNIT_FIELD_DISPLAYID = OBJECT_END + 0x003D, // Size: 1, Type: INT, Flags: PUBLIC
    UNIT_FIELD_NATIVEDISPLAYID = OBJECT_END + 0x003E, // Size: 1, Type: INT, Flags: PUBLIC
    UNIT_FIELD_MOUNTDISPLAYID = OBJECT_END + 0x003F, // Size: 1, Type: INT, Flags: PUBLIC
    UNIT_FIELD_MINDAMAGE = OBJECT_END + 0x0040, // Size: 1, Type: FLOAT, Flags: PRIVATE, OWNER, PARTY_LEADER
    UNIT_FIELD_MAXDAMAGE = OBJECT_END + 0x0041, // Size: 1, Type: FLOAT, Flags: PRIVATE, OWNER, PARTY_LEADER
    UNIT_FIELD_MINOFFHANDDAMAGE = OBJECT_END + 0x0042, // Size: 1, Type: FLOAT, Flags: PRIVATE, OWNER, PARTY_LEADER
    UNIT_FIELD_MAXOFFHANDDAMAGE = OBJECT_END + 0x0043, // Size: 1, Type: FLOAT, Flags: PRIVATE, OWNER, PARTY_LEADER
    UNIT_FIELD_BYTES_1 = OBJECT_END + 0x0044, // Size: 1, Type: BYTES, Flags: PUBLIC
    UNIT_FIELD_PETNUMBER = OBJECT_END + 0x0045, // Size: 1, Type: INT, Flags: PUBLIC
    UNIT_FIELD_PET_NAME_TIMESTAMP = OBJECT_END + 0x0046, // Size: 1, Type: INT, Flags: PUBLIC
    UNIT_FIELD_PETEXPERIENCE = OBJECT_END + 0x0047, // Size: 1, Type: INT, Flags: OWNER
    UNIT_FIELD_PETNEXTLEVELEXP = OBJECT_END + 0x0048, // Size: 1, Type: INT, Flags: OWNER
    UNIT_DYNAMIC_FLAGS = OBJECT_END + 0x0049, // Size: 1, Type: INT, Flags: DYNAMIC
    UNIT_MOD_CAST_SPEED = OBJECT_END + 0x004A, // Size: 1, Type: FLOAT, Flags: PUBLIC
    UNIT_CREATED_BY_SPELL = OBJECT_END + 0x004B, // Size: 1, Type: INT, Flags: PUBLIC
    UNIT_NPC_FLAGS = OBJECT_END + 0x004C, // Size: 1, Type: INT, Flags: DYNAMIC
    UNIT_NPC_EMOTESTATE = OBJECT_END + 0x004D, // Size: 1, Type: INT, Flags: PUBLIC
    UNIT_FIELD_STAT0 = OBJECT_END + 0x004E, // Size: 1, Type: INT, Flags: PRIVATE, OWNER
    UNIT_FIELD_STAT1 = OBJECT_END + 0x004F, // Size: 1, Type: INT, Flags: PRIVATE, OWNER
    UNIT_FIELD_STAT2 = OBJECT_END + 0x0050, // Size: 1, Type: INT, Flags: PRIVATE, OWNER
    UNIT_FIELD_STAT3 = OBJECT_END + 0x0051, // Size: 1, Type: INT, Flags: PRIVATE, OWNER
    UNIT_FIELD_STAT4 = OBJECT_END + 0x0052, // Size: 1, Type: INT, Flags: PRIVATE, OWNER
    UNIT_FIELD_POSSTAT0 = OBJECT_END + 0x0053, // Size: 1, Type: INT, Flags: PRIVATE, OWNER
    UNIT_FIELD_POSSTAT1 = OBJECT_END + 0x0054, // Size: 1, Type: INT, Flags: PRIVATE, OWNER
    UNIT_FIELD_POSSTAT2 = OBJECT_END + 0x0055, // Size: 1, Type: INT, Flags: PRIVATE, OWNER
    UNIT_FIELD_POSSTAT3 = OBJECT_END + 0x0056, // Size: 1, Type: INT, Flags: PRIVATE, OWNER
    UNIT_FIELD_POSSTAT4 = OBJECT_END + 0x0057, // Size: 1, Type: INT, Flags: PRIVATE, OWNER
    UNIT_FIELD_NEGSTAT0 = OBJECT_END + 0x0058, // Size: 1, Type: INT, Flags: PRIVATE, OWNER
    UNIT_FIELD_NEGSTAT1 = OBJECT_END + 0x0059, // Size: 1, Type: INT, Flags: PRIVATE, OWNER
    UNIT_FIELD_NEGSTAT2 = OBJECT_END + 0x005A, // Size: 1, Type: INT, Flags: PRIVATE, OWNER
    UNIT_FIELD_NEGSTAT3 = OBJECT_END + 0x005B, // Size: 1, Type: INT, Flags: PRIVATE, OWNER
    UNIT_FIELD_NEGSTAT4 = OBJECT_END + 0x005C, // Size: 1, Type: INT, Flags: PRIVATE, OWNER
    UNIT_FIELD_RESISTANCES = OBJECT_END + 0x005D, // Size: 7, Type: INT, Flags: PRIVATE, OWNER, PARTY_LEADER
    UNIT_FIELD_RESISTANCEBUFFMODSPOSITIVE = OBJECT_END + 0x0064, // Size: 7, Type: INT, Flags: PRIVATE, OWNER
    UNIT_FIELD_RESISTANCEBUFFMODSNEGATIVE = OBJECT_END + 0x006B, // Size: 7, Type: INT, Flags: PRIVATE, OWNER
    UNIT_FIELD_BASE_MANA = OBJECT_END + 0x0072, // Size: 1, Type: INT, Flags: PUBLIC
    UNIT_FIELD_BASE_HEALTH = OBJECT_END + 0x0073, // Size: 1, Type: INT, Flags: PRIVATE, OWNER
    UNIT_FIELD_BYTES_2 = OBJECT_END + 0x0074, // Size: 1, Type: BYTES, Flags: PUBLIC
    UNIT_FIELD_ATTACK_POWER = OBJECT_END + 0x0075, // Size: 1, Type: INT, Flags: PRIVATE, OWNER
    UNIT_FIELD_ATTACK_POWER_MODS = OBJECT_END + 0x0076, // Size: 1, Type: TWO_SHORT, Flags: PRIVATE, OWNER
    UNIT_FIELD_ATTACK_POWER_MULTIPLIER = OBJECT_END + 0x0077, // Size: 1, Type: FLOAT, Flags: PRIVATE, OWNER
    UNIT_FIELD_RANGED_ATTACK_POWER = OBJECT_END + 0x0078, // Size: 1, Type: INT, Flags: PRIVATE, OWNER
    UNIT_FIELD_RANGED_ATTACK_POWER_MODS = OBJECT_END + 0x0079, // Size: 1, Type: TWO_SHORT, Flags: PRIVATE, OWNER
    UNIT_FIELD_RANGED_ATTACK_POWER_MULTIPLIER = OBJECT_END + 0x007A, // Size: 1, Type: FLOAT, Flags: PRIVATE, OWNER
    UNIT_FIELD_MINRANGEDDAMAGE = OBJECT_END + 0x007B, // Size: 1, Type: FLOAT, Flags: PRIVATE, OWNER
    UNIT_FIELD_MAXRANGEDDAMAGE = OBJECT_END + 0x007C, // Size: 1, Type: FLOAT, Flags: PRIVATE, OWNER
    UNIT_FIELD_POWER_COST_MODIFIER = OBJECT_END + 0x007D, // Size: 7, Type: INT, Flags: PRIVATE, OWNER
    UNIT_FIELD_POWER_COST_MULTIPLIER = OBJECT_END + 0x0084, // Size: 7, Type: FLOAT, Flags: PRIVATE, OWNER
    UNIT_FIELD_MAXHEALTHMODIFIER = OBJECT_END + 0x008B, // Size: 1, Type: FLOAT, Flags: PRIVATE, OWNER
    UNIT_FIELD_HOVERHEIGHT = OBJECT_END + 0x008C, // Size: 1, Type: FLOAT, Flags: PUBLIC
    UNIT_FIELD_PADDING = OBJECT_END + 0x008D, // Size: 1, Type: INT, Flags: NONE
    UNIT_END = OBJECT_END + 0x008E,

    PLAYER_DUEL_ARBITER = UNIT_END + 0x0000, // Size: 2, Type: LONG, Flags: PUBLIC
    PLAYER_FLAGS = UNIT_END + 0x0002, // Size: 1, Type: INT, Flags: PUBLIC
    PLAYER_GUILDID = UNIT_END + 0x0003, // Size: 1, Type: INT, Flags: PUBLIC
    PLAYER_GUILDRANK = UNIT_END + 0x0004, // Size: 1, Type: INT, Flags: PUBLIC
    PLAYER_BYTES = UNIT_END + 0x0005, // Size: 1, Type: BYTES, Flags: PUBLIC
    PLAYER_BYTES_2 = UNIT_END + 0x0006, // Size: 1, Type: BYTES, Flags: PUBLIC
    PLAYER_BYTES_3 = UNIT_END + 0x0007, // Size: 1, Type: BYTES, Flags: PUBLIC
    PLAYER_DUEL_TEAM = UNIT_END + 0x0008, // Size: 1, Type: INT, Flags: PUBLIC
    PLAYER_GUILD_TIMESTAMP = UNIT_END + 0x0009, // Size: 1, Type: INT, Flags: PUBLIC
    PLAYER_QUEST_LOG_1_1 = UNIT_END + 0x000A, // Size: 1, Type: INT, Flags: PARTY_MEMBER
    PLAYER_QUEST_LOG_1_2 = UNIT_END + 0x000B, // Size: 1, Type: INT, Flags: PRIVATE
    PLAYER_QUEST_LOG_1_3 = UNIT_END + 0x000C, // Size: 2, Type: TWO_SHORT, Flags: PRIVATE
    PLAYER_QUEST_LOG_1_4 = UNIT_END + 0x000E, // Size: 1, Type: INT, Flags: PRIVATE
    PLAYER_QUEST_LOG_2_1 = UNIT_END + 0x000F, // Size: 1, Type: INT, Flags: PARTY_MEMBER
    PLAYER_QUEST_LOG_2_2 = UNIT_END + 0x0010, // Size: 1, Type: INT, Flags: PRIVATE
    PLAYER_QUEST_LOG_2_3 = UNIT_END + 0x0011, // Size: 2, Type: TWO_SHORT, Flags: PRIVATE
    PLAYER_QUEST_LOG_2_5 = UNIT_END + 0x0013, // Size: 1, Type: INT, Flags: PRIVATE
    PLAYER_QUEST_LOG_3_1 = UNIT_END + 0x0014, // Size: 1, Type: INT, Flags: PARTY_MEMBER
    PLAYER_QUEST_LOG_3_2 = UNIT_END + 0x0015, // Size: 1, Type: INT, Flags: PRIVATE
    PLAYER_QUEST_LOG_3_3 = UNIT_END + 0x0016, // Size: 2, Type: TWO_SHORT, Flags: PRIVATE
    PLAYER_QUEST_LOG_3_5 = UNIT_END + 0x0018, // Size: 1, Type: INT, Flags: PRIVATE
    PLAYER_QUEST_LOG_4_1 = UNIT_END + 0x0019, // Size: 1, Type: INT, Flags: PARTY_MEMBER
    PLAYER_QUEST_LOG_4_2 = UNIT_END + 0x001A, // Size: 1, Type: INT, Flags: PRIVATE
    PLAYER_QUEST_LOG_4_3 = UNIT_END + 0x001B, // Size: 2, Type: TWO_SHORT, Flags: PRIVATE
    PLAYER_QUEST_LOG_4_5 = UNIT_END + 0x001D, // Size: 1, Type: INT, Flags: PRIVATE
    PLAYER_QUEST_LOG_5_1 = UNIT_END + 0x001E, // Size: 1, Type: INT, Flags: PARTY_MEMBER
    PLAYER_QUEST_LOG_5_2 = UNIT_END + 0x001F, // Size: 1, Type: INT, Flags: PRIVATE
    PLAYER_QUEST_LOG_5_3 = UNIT_END + 0x0020, // Size: 2, Type: TWO_SHORT, Flags: PRIVATE
    PLAYER_QUEST_LOG_5_5 = UNIT_END + 0x0022, // Size: 1, Type: INT, Flags: PRIVATE
    PLAYER_QUEST_LOG_6_1 = UNIT_END + 0x0023, // Size: 1, Type: INT, Flags: PARTY_MEMBER
    PLAYER_QUEST_LOG_6_2 = UNIT_END + 0x0024, // Size: 1, Type: INT, Flags: PRIVATE
    PLAYER_QUEST_LOG_6_3 = UNIT_END + 0x0025, // Size: 2, Type: TWO_SHORT, Flags: PRIVATE
    PLAYER_QUEST_LOG_6_5 = UNIT_END + 0x0027, // Size: 1, Type: INT, Flags: PRIVATE
    PLAYER_QUEST_LOG_7_1 = UNIT_END + 0x0028, // Size: 1, Type: INT, Flags: PARTY_MEMBER
    PLAYER_QUEST_LOG_7_2 = UNIT_END + 0x0029, // Size: 1, Type: INT, Flags: PRIVATE
    PLAYER_QUEST_LOG_7_3 = UNIT_END + 0x002A, // Size: 2, Type: TWO_SHORT, Flags: PRIVATE
    PLAYER_QUEST_LOG_7_5 = UNIT_END + 0x002C, // Size: 1, Type: INT, Flags: PRIVATE
    PLAYER_QUEST_LOG_8_1 = UNIT_END + 0x002D, // Size: 1, Type: INT, Flags: PARTY_MEMBER
    PLAYER_QUEST_LOG_8_2 = UNIT_END + 0x002E, // Size: 1, Type: INT, Flags: PRIVATE
    PLAYER_QUEST_LOG_8_3 = UNIT_END + 0x002F, // Size: 2, Type: TWO_SHORT, Flags: PRIVATE
    PLAYER_QUEST_LOG_8_5 = UNIT_END + 0x0031, // Size: 1, Type: INT, Flags: PRIVATE
    PLAYER_QUEST_LOG_9_1 = UNIT_END + 0x0032, // Size: 1, Type: INT, Flags: PARTY_MEMBER
    PLAYER_QUEST_LOG_9_2 = UNIT_END + 0x0033, // Size: 1, Type: INT, Flags: PRIVATE
    PLAYER_QUEST_LOG_9_3 = UNIT_END + 0x0034, // Size: 2, Type: TWO_SHORT, Flags: PRIVATE
    PLAYER_QUEST_LOG_9_5 = UNIT_END + 0x0036, // Size: 1, Type: INT, Flags: PRIVATE
    PLAYER_QUEST_LOG_10_1 = UNIT_END + 0x0037, // Size: 1, Type: INT, Flags: PARTY_MEMBER
    PLAYER_QUEST_LOG_10_2 = UNIT_END + 0x0038, // Size: 1, Type: INT, Flags: PRIVATE
    PLAYER_QUEST_LOG_10_3 = UNIT_END + 0x0039, // Size: 2, Type: TWO_SHORT, Flags: PRIVATE
    PLAYER_QUEST_LOG_10_5 = UNIT_END + 0x003B, // Size: 1, Type: INT, Flags: PRIVATE
    PLAYER_QUEST_LOG_11_1 = UNIT_END + 0x003C, // Size: 1, Type: INT, Flags: PARTY_MEMBER
    PLAYER_QUEST_LOG_11_2 = UNIT_END + 0x003D, // Size: 1, Type: INT, Flags: PRIVATE
    PLAYER_QUEST_LOG_11_3 = UNIT_END + 0x003E, // Size: 2, Type: TWO_SHORT, Flags: PRIVATE
    PLAYER_QUEST_LOG_11_5 = UNIT_END + 0x0040, // Size: 1, Type: INT, Flags: PRIVATE
    PLAYER_QUEST_LOG_12_1 = UNIT_END + 0x0041, // Size: 1, Type: INT, Flags: PARTY_MEMBER
    PLAYER_QUEST_LOG_12_2 = UNIT_END + 0x0042, // Size: 1, Type: INT, Flags: PRIVATE
    PLAYER_QUEST_LOG_12_3 = UNIT_END + 0x0043, // Size: 2, Type: TWO_SHORT, Flags: PRIVATE
    PLAYER_QUEST_LOG_12_5 = UNIT_END + 0x0045, // Size: 1, Type: INT, Flags: PRIVATE
    PLAYER_QUEST_LOG_13_1 = UNIT_END + 0x0046, // Size: 1, Type: INT, Flags: PARTY_MEMBER
    PLAYER_QUEST_LOG_13_2 = UNIT_END + 0x0047, // Size: 1, Type: INT, Flags: PRIVATE
    PLAYER_QUEST_LOG_13_3 = UNIT_END + 0x0048, // Size: 2, Type: TWO_SHORT, Flags: PRIVATE
    PLAYER_QUEST_LOG_13_5 = UNIT_END + 0x004A, // Size: 1, Type: INT, Flags: PRIVATE
    PLAYER_QUEST_LOG_14_1 = UNIT_END + 0x004B, // Size: 1, Type: INT, Flags: PARTY_MEMBER
    PLAYER_QUEST_LOG_14_2 = UNIT_END + 0x004C, // Size: 1, Type: INT, Flags: PRIVATE
    PLAYER_QUEST_LOG_14_3 = UNIT_END + 0x004D, // Size: 2, Type: TWO_SHORT, Flags: PRIVATE
    PLAYER_QUEST_LOG_14_5 = UNIT_END + 0x004F, // Size: 1, Type: INT, Flags: PRIVATE
    PLAYER_QUEST_LOG_15_1 = UNIT_END + 0x0050, // Size: 1, Type: INT, Flags: PARTY_MEMBER
    PLAYER_QUEST_LOG_15_2 = UNIT_END + 0x0051, // Size: 1, Type: INT, Flags: PRIVATE
    PLAYER_QUEST_LOG_15_3 = UNIT_END + 0x0052, // Size: 2, Type: TWO_SHORT, Flags: PRIVATE
    PLAYER_QUEST_LOG_15_5 = UNIT_END + 0x0054, // Size: 1, Type: INT, Flags: PRIVATE
    PLAYER_QUEST_LOG_16_1 = UNIT_END + 0x0055, // Size: 1, Type: INT, Flags: PARTY_MEMBER
    PLAYER_QUEST_LOG_16_2 = UNIT_END + 0x0056, // Size: 1, Type: INT, Flags: PRIVATE
    PLAYER_QUEST_LOG_16_3 = UNIT_END + 0x0057, // Size: 2, Type: TWO_SHORT, Flags: PRIVATE
    PLAYER_QUEST_LOG_16_5 = UNIT_END + 0x0059, // Size: 1, Type: INT, Flags: PRIVATE
    PLAYER_QUEST_LOG_17_1 = UNIT_END + 0x005A, // Size: 1, Type: INT, Flags: PARTY_MEMBER
    PLAYER_QUEST_LOG_17_2 = UNIT_END + 0x005B, // Size: 1, Type: INT, Flags: PRIVATE
    PLAYER_QUEST_LOG_17_3 = UNIT_END + 0x005C, // Size: 2, Type: TWO_SHORT, Flags: PRIVATE
    PLAYER_QUEST_LOG_17_5 = UNIT_END + 0x005E, // Size: 1, Type: INT, Flags: PRIVATE
    PLAYER_QUEST_LOG_18_1 = UNIT_END + 0x005F, // Size: 1, Type: INT, Flags: PARTY_MEMBER
    PLAYER_QUEST_LOG_18_2 = UNIT_END + 0x0060, // Size: 1, Type: INT, Flags: PRIVATE
    PLAYER_QUEST_LOG_18_3 = UNIT_END + 0x0061, // Size: 2, Type: TWO_SHORT, Flags: PRIVATE
    PLAYER_QUEST_LOG_18_5 = UNIT_END + 0x0063, // Size: 1, Type: INT, Flags: PRIVATE
    PLAYER_QUEST_LOG_19_1 = UNIT_END + 0x0064, // Size: 1, Type: INT, Flags: PARTY_MEMBER
    PLAYER_QUEST_LOG_19_2 = UNIT_END + 0x0065, // Size: 1, Type: INT, Flags: PRIVATE
    PLAYER_QUEST_LOG_19_3 = UNIT_END + 0x0066, // Size: 2, Type: TWO_SHORT, Flags: PRIVATE
    PLAYER_QUEST_LOG_19_5 = UNIT_END + 0x0068, // Size: 1, Type: INT, Flags: PRIVATE
    PLAYER_QUEST_LOG_20_1 = UNIT_END + 0x0069, // Size: 1, Type: INT, Flags: PARTY_MEMBER
    PLAYER_QUEST_LOG_20_2 = UNIT_END + 0x006A, // Size: 1, Type: INT, Flags: PRIVATE
    PLAYER_QUEST_LOG_20_3 = UNIT_END + 0x006B, // Size: 2, Type: TWO_SHORT, Flags: PRIVATE
    PLAYER_QUEST_LOG_20_5 = UNIT_END + 0x006D, // Size: 1, Type: INT, Flags: PRIVATE
    PLAYER_QUEST_LOG_21_1 = UNIT_END + 0x006E, // Size: 1, Type: INT, Flags: PARTY_MEMBER
    PLAYER_QUEST_LOG_21_2 = UNIT_END + 0x006F, // Size: 1, Type: INT, Flags: PRIVATE
    PLAYER_QUEST_LOG_21_3 = UNIT_END + 0x0070, // Size: 2, Type: TWO_SHORT, Flags: PRIVATE
    PLAYER_QUEST_LOG_21_5 = UNIT_END + 0x0072, // Size: 1, Type: INT, Flags: PRIVATE
    PLAYER_QUEST_LOG_22_1 = UNIT_END + 0x0073, // Size: 1, Type: INT, Flags: PARTY_MEMBER
    PLAYER_QUEST_LOG_22_2 = UNIT_END + 0x0074, // Size: 1, Type: INT, Flags: PRIVATE
    PLAYER_QUEST_LOG_22_3 = UNIT_END + 0x0075, // Size: 2, Type: TWO_SHORT, Flags: PRIVATE
    PLAYER_QUEST_LOG_22_5 = UNIT_END + 0x0077, // Size: 1, Type: INT, Flags: PRIVATE
    PLAYER_QUEST_LOG_23_1 = UNIT_END + 0x0078, // Size: 1, Type: INT, Flags: PARTY_MEMBER
    PLAYER_QUEST_LOG_23_2 = UNIT_END + 0x0079, // Size: 1, Type: INT, Flags: PRIVATE
    PLAYER_QUEST_LOG_23_3 = UNIT_END + 0x007A, // Size: 2, Type: TWO_SHORT, Flags: PRIVATE
    PLAYER_QUEST_LOG_23_5 = UNIT_END + 0x007C, // Size: 1, Type: INT, Flags: PRIVATE
    PLAYER_QUEST_LOG_24_1 = UNIT_END + 0x007D, // Size: 1, Type: INT, Flags: PARTY_MEMBER
    PLAYER_QUEST_LOG_24_2 = UNIT_END + 0x007E, // Size: 1, Type: INT, Flags: PRIVATE
    PLAYER_QUEST_LOG_24_3 = UNIT_END + 0x007F, // Size: 2, Type: TWO_SHORT, Flags: PRIVATE
    PLAYER_QUEST_LOG_24_5 = UNIT_END + 0x0081, // Size: 1, Type: INT, Flags: PRIVATE
    PLAYER_QUEST_LOG_25_1 = UNIT_END + 0x0082, // Size: 1, Type: INT, Flags: PARTY_MEMBER
    PLAYER_QUEST_LOG_25_2 = UNIT_END + 0x0083, // Size: 1, Type: INT, Flags: PRIVATE
    PLAYER_QUEST_LOG_25_3 = UNIT_END + 0x0084, // Size: 2, Type: TWO_SHORT, Flags: PRIVATE
    PLAYER_QUEST_LOG_25_5 = UNIT_END + 0x0086, // Size: 1, Type: INT, Flags: PRIVATE
    PLAYER_VISIBLE_ITEM_1_ENTRYID = UNIT_END + 0x0087, // Size: 1, Type: INT, Flags: PUBLIC
    PLAYER_VISIBLE_ITEM_1_ENCHANTMENT = UNIT_END + 0x0088, // Size: 1, Type: TWO_SHORT, Flags: PUBLIC
    PLAYER_VISIBLE_ITEM_2_ENTRYID = UNIT_END + 0x0089, // Size: 1, Type: INT, Flags: PUBLIC
    PLAYER_VISIBLE_ITEM_2_ENCHANTMENT = UNIT_END + 0x008A, // Size: 1, Type: TWO_SHORT, Flags: PUBLIC
    PLAYER_VISIBLE_ITEM_3_ENTRYID = UNIT_END + 0x008B, // Size: 1, Type: INT, Flags: PUBLIC
    PLAYER_VISIBLE_ITEM_3_ENCHANTMENT = UNIT_END + 0x008C, // Size: 1, Type: TWO_SHORT, Flags: PUBLIC
    PLAYER_VISIBLE_ITEM_4_ENTRYID = UNIT_END + 0x008D, // Size: 1, Type: INT, Flags: PUBLIC
    PLAYER_VISIBLE_ITEM_4_ENCHANTMENT = UNIT_END + 0x008E, // Size: 1, Type: TWO_SHORT, Flags: PUBLIC
    PLAYER_VISIBLE_ITEM_5_ENTRYID = UNIT_END + 0x008F, // Size: 1, Type: INT, Flags: PUBLIC
    PLAYER_VISIBLE_ITEM_5_ENCHANTMENT = UNIT_END + 0x0090, // Size: 1, Type: TWO_SHORT, Flags: PUBLIC
    PLAYER_VISIBLE_ITEM_6_ENTRYID = UNIT_END + 0x0091, // Size: 1, Type: INT, Flags: PUBLIC
    PLAYER_VISIBLE_ITEM_6_ENCHANTMENT = UNIT_END + 0x0092, // Size: 1, Type: TWO_SHORT, Flags: PUBLIC
    PLAYER_VISIBLE_ITEM_7_ENTRYID = UNIT_END + 0x0093, // Size: 1, Type: INT, Flags: PUBLIC
    PLAYER_VISIBLE_ITEM_7_ENCHANTMENT = UNIT_END + 0x0094, // Size: 1, Type: TWO_SHORT, Flags: PUBLIC
    PLAYER_VISIBLE_ITEM_8_ENTRYID = UNIT_END + 0x0095, // Size: 1, Type: INT, Flags: PUBLIC
    PLAYER_VISIBLE_ITEM_8_ENCHANTMENT = UNIT_END + 0x0096, // Size: 1, Type: TWO_SHORT, Flags: PUBLIC
    PLAYER_VISIBLE_ITEM_9_ENTRYID = UNIT_END + 0x0097, // Size: 1, Type: INT, Flags: PUBLIC
    PLAYER_VISIBLE_ITEM_9_ENCHANTMENT = UNIT_END + 0x0098, // Size: 1, Type: TWO_SHORT, Flags: PUBLIC
    PLAYER_VISIBLE_ITEM_10_ENTRYID = UNIT_END + 0x0099, // Size: 1, Type: INT, Flags: PUBLIC
    PLAYER_VISIBLE_ITEM_10_ENCHANTMENT = UNIT_END + 0x009A, // Size: 1, Type: TWO_SHORT, Flags: PUBLIC
    PLAYER_VISIBLE_ITEM_11_ENTRYID = UNIT_END + 0x009B, // Size: 1, Type: INT, Flags: PUBLIC
    PLAYER_VISIBLE_ITEM_11_ENCHANTMENT = UNIT_END + 0x009C, // Size: 1, Type: TWO_SHORT, Flags: PUBLIC
    PLAYER_VISIBLE_ITEM_12_ENTRYID = UNIT_END + 0x009D, // Size: 1, Type: INT, Flags: PUBLIC
    PLAYER_VISIBLE_ITEM_12_ENCHANTMENT = UNIT_END + 0x009E, // Size: 1, Type: TWO_SHORT, Flags: PUBLIC
    PLAYER_VISIBLE_ITEM_13_ENTRYID = UNIT_END + 0x009F, // Size: 1, Type: INT, Flags: PUBLIC
    PLAYER_VISIBLE_ITEM_13_ENCHANTMENT = UNIT_END + 0x00A0, // Size: 1, Type: TWO_SHORT, Flags: PUBLIC
    PLAYER_VISIBLE_ITEM_14_ENTRYID = UNIT_END + 0x00A1, // Size: 1, Type: INT, Flags: PUBLIC
    PLAYER_VISIBLE_ITEM_14_ENCHANTMENT = UNIT_END + 0x00A2, // Size: 1, Type: TWO_SHORT, Flags: PUBLIC
    PLAYER_VISIBLE_ITEM_15_ENTRYID = UNIT_END + 0x00A3, // Size: 1, Type: INT, Flags: PUBLIC
    PLAYER_VISIBLE_ITEM_15_ENCHANTMENT = UNIT_END + 0x00A4, // Size: 1, Type: TWO_SHORT, Flags: PUBLIC
    PLAYER_VISIBLE_ITEM_16_ENTRYID = UNIT_END + 0x00A5, // Size: 1, Type: INT, Flags: PUBLIC
    PLAYER_VISIBLE_ITEM_16_ENCHANTMENT = UNIT_END + 0x00A6, // Size: 1, Type: TWO_SHORT, Flags: PUBLIC
    PLAYER_VISIBLE_ITEM_17_ENTRYID = UNIT_END + 0x00A7, // Size: 1, Type: INT, Flags: PUBLIC
    PLAYER_VISIBLE_ITEM_17_ENCHANTMENT = UNIT_END + 0x00A8, // Size: 1, Type: TWO_SHORT, Flags: PUBLIC
    PLAYER_VISIBLE_ITEM_18_ENTRYID = UNIT_END + 0x00A9, // Size: 1, Type: INT, Flags: PUBLIC
    PLAYER_VISIBLE_ITEM_18_ENCHANTMENT = UNIT_END + 0x00AA, // Size: 1, Type: TWO_SHORT, Flags: PUBLIC
    PLAYER_VISIBLE_ITEM_19_ENTRYID = UNIT_END + 0x00AB, // Size: 1, Type: INT, Flags: PUBLIC
    PLAYER_VISIBLE_ITEM_19_ENCHANTMENT = UNIT_END + 0x00AC, // Size: 1, Type: TWO_SHORT, Flags: PUBLIC
    PLAYER_CHOSEN_TITLE = UNIT_END + 0x00AD, // Size: 1, Type: INT, Flags: PUBLIC
    PLAYER_FAKE_INEBRIATION = UNIT_END + 0x00AE, // Size: 1, Type: INT, Flags: PUBLIC
    PLAYER_FIELD_PAD_0 = UNIT_END + 0x00AF, // Size: 1, Type: INT, Flags: NONE
    PLAYER_FIELD_INV_SLOT_HEAD = UNIT_END + 0x00B0, // Size: 46, Type: LONG, Flags: PRIVATE
    PLAYER_FIELD_PACK_SLOT_1 = UNIT_END + 0x00DE, // Size: 32, Type: LONG, Flags: PRIVATE
    PLAYER_FIELD_BANK_SLOT_1 = UNIT_END + 0x00FE, // Size: 56, Type: LONG, Flags: PRIVATE
    PLAYER_FIELD_BANKBAG_SLOT_1 = UNIT_END + 0x0136, // Size: 14, Type: LONG, Flags: PRIVATE
    PLAYER_FIELD_VENDORBUYBACK_SLOT_1 = UNIT_END + 0x0144, // Size: 24, Type: LONG, Flags: PRIVATE
    PLAYER_FIELD_KEYRING_SLOT_1 = UNIT_END + 0x015C, // Size: 64, Type: LONG, Flags: PRIVATE
    PLAYER_FIELD_CURRENCYTOKEN_SLOT_1 = UNIT_END + 0x019C, // Size: 64, Type: LONG, Flags: PRIVATE
    PLAYER_FARSIGHT = UNIT_END + 0x01DC, // Size: 2, Type: LONG, Flags: PRIVATE
    PLAYER__FIELD_KNOWN_TITLES = UNIT_END + 0x01DE, // Size: 2, Type: LONG, Flags: PRIVATE
    PLAYER__FIELD_KNOWN_TITLES1 = UNIT_END + 0x01E0, // Size: 2, Type: LONG, Flags: PRIVATE
    PLAYER__FIELD_KNOWN_TITLES2 = UNIT_END + 0x01E2, // Size: 2, Type: LONG, Flags: PRIVATE
    PLAYER_FIELD_KNOWN_CURRENCIES = UNIT_END + 0x01E4, // Size: 2, Type: LONG, Flags: PRIVATE
    PLAYER_XP = UNIT_END + 0x01E6, // Size: 1, Type: INT, Flags: PRIVATE
    PLAYER_NEXT_LEVEL_XP = UNIT_END + 0x01E7, // Size: 1, Type: INT, Flags: PRIVATE
    PLAYER_SKILL_INFO_1_1 = UNIT_END + 0x01E8, // Size: 384, Type: TWO_SHORT, Flags: PRIVATE
    PLAYER_CHARACTER_POINTS1 = UNIT_END + 0x0368, // Size: 1, Type: INT, Flags: PRIVATE
    PLAYER_CHARACTER_POINTS2 = UNIT_END + 0x0369, // Size: 1, Type: INT, Flags: PRIVATE
    PLAYER_TRACK_CREATURES = UNIT_END + 0x036A, // Size: 1, Type: INT, Flags: PRIVATE
    PLAYER_TRACK_RESOURCES = UNIT_END + 0x036B, // Size: 1, Type: INT, Flags: PRIVATE
    PLAYER_BLOCK_PERCENTAGE = UNIT_END + 0x036C, // Size: 1, Type: FLOAT, Flags: PRIVATE
    PLAYER_DODGE_PERCENTAGE = UNIT_END + 0x036D, // Size: 1, Type: FLOAT, Flags: PRIVATE
    PLAYER_PARRY_PERCENTAGE = UNIT_END + 0x036E, // Size: 1, Type: FLOAT, Flags: PRIVATE
    PLAYER_EXPERTISE = UNIT_END + 0x036F, // Size: 1, Type: INT, Flags: PRIVATE
    PLAYER_OFFHAND_EXPERTISE = UNIT_END + 0x0370, // Size: 1, Type: INT, Flags: PRIVATE
    PLAYER_CRIT_PERCENTAGE = UNIT_END + 0x0371, // Size: 1, Type: FLOAT, Flags: PRIVATE
    PLAYER_RANGED_CRIT_PERCENTAGE = UNIT_END + 0x0372, // Size: 1, Type: FLOAT, Flags: PRIVATE
    PLAYER_OFFHAND_CRIT_PERCENTAGE = UNIT_END + 0x0373, // Size: 1, Type: FLOAT, Flags: PRIVATE
    PLAYER_SPELL_CRIT_PERCENTAGE1 = UNIT_END + 0x0374, // Size: 7, Type: FLOAT, Flags: PRIVATE
    PLAYER_SHIELD_BLOCK = UNIT_END + 0x037B, // Size: 1, Type: INT, Flags: PRIVATE
    PLAYER_SHIELD_BLOCK_CRIT_PERCENTAGE = UNIT_END + 0x037C, // Size: 1, Type: FLOAT, Flags: PRIVATE
    PLAYER_EXPLORED_ZONES_1 = UNIT_END + 0x037D, // Size: 128, Type: BYTES, Flags: PRIVATE
    PLAYER_REST_STATE_EXPERIENCE = UNIT_END + 0x03FD, // Size: 1, Type: INT, Flags: PRIVATE
    PLAYER_FIELD_COINAGE = UNIT_END + 0x03FE, // Size: 1, Type: INT, Flags: PRIVATE
    PLAYER_FIELD_MOD_DAMAGE_DONE_POS = UNIT_END + 0x03FF, // Size: 7, Type: INT, Flags: PRIVATE
    PLAYER_FIELD_MOD_DAMAGE_DONE_NEG = UNIT_END + 0x0406, // Size: 7, Type: INT, Flags: PRIVATE
    PLAYER_FIELD_MOD_DAMAGE_DONE_PCT = UNIT_END + 0x040D, // Size: 7, Type: INT, Flags: PRIVATE
    PLAYER_FIELD_MOD_HEALING_DONE_POS = UNIT_END + 0x0414, // Size: 1, Type: INT, Flags: PRIVATE
    PLAYER_FIELD_MOD_HEALING_PCT = UNIT_END + 0x0415, // Size: 1, Type: FLOAT, Flags: PRIVATE
    PLAYER_FIELD_MOD_HEALING_DONE_PCT = UNIT_END + 0x0416, // Size: 1, Type: FLOAT, Flags: PRIVATE
    PLAYER_FIELD_MOD_TARGET_RESISTANCE = UNIT_END + 0x0417, // Size: 1, Type: INT, Flags: PRIVATE
    PLAYER_FIELD_MOD_TARGET_PHYSICAL_RESISTANCE = UNIT_END + 0x0418, // Size: 1, Type: INT, Flags: PRIVATE
    PLAYER_FIELD_BYTES = UNIT_END + 0x0419, // Size: 1, Type: BYTES, Flags: PRIVATE
    PLAYER_AMMO_ID = UNIT_END + 0x041A, // Size: 1, Type: INT, Flags: PRIVATE
    PLAYER_SELF_RES_SPELL = UNIT_END + 0x041B, // Size: 1, Type: INT, Flags: PRIVATE
    PLAYER_FIELD_PVP_MEDALS = UNIT_END + 0x041C, // Size: 1, Type: INT, Flags: PRIVATE
    PLAYER_FIELD_BUYBACK_PRICE_1 = UNIT_END + 0x041D, // Size: 12, Type: INT, Flags: PRIVATE
    PLAYER_FIELD_BUYBACK_TIMESTAMP_1 = UNIT_END + 0x0429, // Size: 12, Type: INT, Flags: PRIVATE
    PLAYER_FIELD_KILLS = UNIT_END + 0x0435, // Size: 1, Type: TWO_SHORT, Flags: PRIVATE
    PLAYER_FIELD_TODAY_CONTRIBUTION = UNIT_END + 0x0436, // Size: 1, Type: INT, Flags: PRIVATE
    PLAYER_FIELD_YESTERDAY_CONTRIBUTION = UNIT_END + 0x0437, // Size: 1, Type: INT, Flags: PRIVATE
    PLAYER_FIELD_LIFETIME_HONORBALE_KILLS = UNIT_END + 0x0438, // Size: 1, Type: INT, Flags: PRIVATE
    PLAYER_FIELD_BYTES2 = UNIT_END + 0x0439, // Size: 1, Type: 6, Flags: PRIVATE
    PLAYER_FIELD_WATCHED_FACTION_INDEX = UNIT_END + 0x043A, // Size: 1, Type: INT, Flags: PRIVATE
    PLAYER_FIELD_COMBAT_RATING_1 = UNIT_END + 0x043B, // Size: 25, Type: INT, Flags: PRIVATE
    PLAYER_FIELD_ARENA_TEAM_INFO_1_1 = UNIT_END + 0x0454, // Size: 21, Type: INT, Flags: PRIVATE
    PLAYER_FIELD_HONOR_CURRENCY = UNIT_END + 0x0469, // Size: 1, Type: INT, Flags: PRIVATE
    PLAYER_FIELD_ARENA_CURRENCY = UNIT_END + 0x046A, // Size: 1, Type: INT, Flags: PRIVATE
    PLAYER_FIELD_MAX_LEVEL = UNIT_END + 0x046B, // Size: 1, Type: INT, Flags: PRIVATE
    PLAYER_FIELD_DAILY_QUESTS_1 = UNIT_END + 0x046C, // Size: 25, Type: INT, Flags: PRIVATE
    PLAYER_RUNE_REGEN_1 = UNIT_END + 0x0485, // Size: 4, Type: FLOAT, Flags: PRIVATE
    PLAYER_NO_REAGENT_COST_1 = UNIT_END + 0x0489, // Size: 3, Type: INT, Flags: PRIVATE
    PLAYER_FIELD_GLYPH_SLOTS_1 = UNIT_END + 0x048C, // Size: 6, Type: INT, Flags: PRIVATE
    PLAYER_FIELD_GLYPHS_1 = UNIT_END + 0x0492, // Size: 6, Type: INT, Flags: PRIVATE
    PLAYER_GLYPHS_ENABLED = UNIT_END + 0x0498, // Size: 1, Type: INT, Flags: PRIVATE
    PLAYER_PET_SPELL_POWER = UNIT_END + 0x0499, // Size: 1, Type: INT, Flags: PRIVATE
    PLAYER_END = UNIT_END + 0x049A,
};

enum EGoFields
{
    GO_FLAG_NONE = 0x00000000,
    GO_FLAG_IN_USE = 0x00000001,
    GO_FLAG_LOCKED = 0x00000002,
    GO_FLAG_INTERACT_COND = 0x00000004,
    GO_FLAG_TRANSPORT = 0x00000008,
    GO_FLAG_NOT_SELECTABLE = 0x00000010,
    GO_FLAG_NODESPAWN = 0x00000020,
    GO_FLAG_TRIGGERED = 0x00000040,
    GO_FLAG_DAMAGED = 0x00000200,
    GO_FLAG_DESTROYED = 0x00000400,
};

struct C3Vector
{
    float X, Y, Z;
};

enum UnitDynFlags
{
    UNIT_DYNFLAG_NONE = 0x0000,
    UNIT_DYNFLAG_LOOTABLE = 0x0001,
    UNIT_DYNFLAG_TRACK_UNIT = 0x0002,
    UNIT_DYNFLAG_TAPPED = 0x0004,       // Lua_UnitIsTapped
    UNIT_DYNFLAG_TAPPED_BY_PLAYER = 0x0008,       // Lua_UnitIsTappedByPlayer
    UNIT_DYNFLAG_SPECIALINFO = 0x0010,
    UNIT_DYNFLAG_DEAD = 0x0020,
    UNIT_DYNFLAG_REFER_A_FRIEND = 0x0040,
    UNIT_DYNFLAG_TAPPED_BY_ALL_THREAT_LIST = 0x0080        // Lua_UnitIsTappedByAllThreatList
};

struct ObjectEntry {
    guid_t guid;
    int type;
    int entry;
    float scaleX;
    int padding;
};
static_assert(sizeof(ObjectEntry) == 0x18);

struct UnitEntry : ObjectEntry {
    guid_t charm;
    guid_t summon;
    guid_t critter;
    guid_t charmedBy;
    guid_t summonedBy;
    guid_t createdBy;
    guid_t target;
    guid_t channelObject;
    uint32_t channelSpell;
    uint32_t bytes0;
    uint32_t health;
    uint32_t power[7];
    uint32_t maxHealth;
    uint32_t maxPower[7];
    uint32_t powerRegenFlatModifier[7];
    uint32_t powerRegenInterruptedFlatModifier[7];
    uint32_t level;
    uint32_t factionTemplate;
    uint32_t virtualItemSlotId[3];
    uint32_t flags;
    uint32_t flags2;
    float auraState;
    uint32_t baseAttackTime[2];
    uint32_t rangedAttackTime;
    float boundingRadius;
    float combatReach;
    uint32_t displayId;
    uint32_t nativeDisplayId;
    uint32_t mountDisplayId;
    float minDamage;
    float maxDamage;
    float minOffhandDamage;
    float maxOffhandDamage;
    uint32_t bytes1;
    uint32_t petNumber;
    uint32_t petNameTimestamp;
    uint32_t petExperience;
    uint32_t petNextLevelExp;
    uint32_t dynamicFlags;
    float modCastSpeed;
    uint32_t createdBySpell;
    uint32_t npc_flags;
    char gap1[0xFE];
};
static_assert(sizeof(UnitEntry) == 0x250);

struct PlayerQuest {
    int a1, a2, a3, a4, a5;
};
static_assert(sizeof(PlayerQuest) == 0x14);

struct PlayerVisibleItem {
    int entryId;
    int enchant;
};
static_assert(sizeof(PlayerVisibleItem) == 0x8);

struct PlayerEntry : UnitEntry {
    UnitEntry unit;
    guid_t duelArbiter;
    uint32_t flags;
    uint32_t guildId, guildRank;
    uint32_t bytes1, bytes2, bytes3;
    uint32_t duelTeam;
    uint32_t guildTimestamp;
    PlayerQuest quests[25];
    PlayerVisibleItem visibleItems[19];
};

struct ObjectVtbl {
    DWORD gap0[11];
    void(__thiscall* GetPosition)(Object* self, VecXYZ* pos);
};

struct UnitVtbl {
    DWORD gap0[11];
    void(__thiscall* GetPosition)(Unit* self, VecXYZ* pos);
};

struct PlayerVtbl {};

struct Object {
    ObjectVtbl* vmt;
    int field4;
    ObjectEntry* entry;
};

struct Unit {
    UnitVtbl* vmt;
    int field4;
    UnitEntry* entry;
    uint32_t gap[779];
    Frame* nameplate;

    inline Object* ToObject() { return (Object*)this; }
};

struct Player {
    PlayerVtbl* vmt;
    int field4;
    PlayerEntry* entry;

    inline Unit* ToUnit() { return (Unit*)this; }
};

// Base


class CGObject_C // sizeof(CGObject_C) == 0xD0
{
public:
    template <typename T> T& GetValue(uint32_t index) const { return *((T*)&m_data[index]); }

    void SetValueBytes(uint32_t index, uint8_t offset, uint8_t value)
    {
        if (!m_data) return;
        if (offset >= 4) return;

        uint32_t& current = m_data[index];

        uint8_t currentByte = (current >> (offset * 8)) & 0xFF;
        if (currentByte != value)
        {
            current &= ~(0xFFu << (offset * 8));
            current |= (uint32_t(value) << (offset * 8));
        }
    }

    virtual ~CGObject_C(); // 0
    virtual void Disable(); // 1
    virtual void Reenable(); // 2
    virtual void PostReenable(); // 3
    virtual void HandleOutOfRange(); // 4
    virtual void UpdateWorldObject(); // 5
    virtual void ShouldFadeout(); // 6
    virtual void UpdateDisplayInfo(); // 7
    virtual void GetNamePosition(); // 8
    virtual void GetBag(); // 9
    virtual void GetBag2(); // 10
    virtual C3Vector& GetPosition(C3Vector& pos); // 11
    virtual C3Vector& GetRawPosition(C3Vector& pos); // 12
    virtual float GetFacing(); // 13
    virtual float GetRawFacing(); // 14
    virtual float GetScale(); // 15
    virtual uint64_t GetTransportGUID(); // 16
    virtual void GetRotation(); // 17
    virtual void SetFrameOfReference(); // 18
    virtual bool IsQuestGiver(); // 19
    virtual void RefreshInteractIcon(); // 20
    virtual void UpdateInteractIcon(); // 21
    virtual void UpdateInteractIconAttach(); // 22
    virtual void UpdateInteractIconScale(); // 23
    virtual bool GetModelFileName(char const** modelFileName); // 24
    virtual void ScaleChangeUpdate(); // 25
    virtual void ScaleChangeFinished(); // 26
    virtual void RenderTargetSelection(); // 27
    virtual void RenderPetTargetSelection(); // 28
    virtual void Render(); // 29
    virtual void GetSelectionHighlightColor(); // 30
    virtual float GetTrueScale(); // 31
    virtual void ModelLoaded(); // 32
    virtual void ApplyAlpha(); // 33
    virtual void PreAnimate(); // 34
    virtual void Animate(); // 35
    virtual void ShouldRender(); // 36
    virtual float GetRenderFacing(); // 37
    virtual void OnSpecialMountAnim(); // 38
    virtual bool IsSolidSelectable(); // 39
    virtual void Dummy40(); // 40
    virtual bool CanHighlight(); // 41
    virtual bool CanBeTargetted(); // 42
    virtual void FloatingTooltip(); // 43
    virtual void OnRightClick(); // 44
    virtual bool IsHighlightSuppressed(); // 45
    virtual void OnSpellEffectClear(); // 46
    virtual void GetAppropriateSpellVisual(); // 47
    virtual void ConnectToLightningThisFrame(); // 48
    virtual void GetMatrix(); // 49
    virtual void ObjectNameVisibilityChanged(); // 50
    virtual void UpdateObjectNameString(); // 51
    virtual void ShouldRenderObjectName(); // 52
    virtual void GetObjectModel(); // 53
    virtual const char* GetObjectName(); // 54
    virtual void GetPageTextID(); // 55
    virtual void CleanUpVehicleBoneAnimsBeforeObjectModelChange(); // 56
    virtual void ShouldFadeIn(); // 57
    virtual float GetBaseAlpha(); // 58
    virtual bool IsTransport(); // 59
    virtual bool IsPointInside(); // 60
    virtual void AddPassenger(); // 61
    virtual float GetSpeed(); // 62
    virtual void PlaySpellVisualKit_PlayAnims(); // 63
    virtual void PlaySpellVisualKit_HandleWeapons(); // 64
    virtual void PlaySpellVisualKit_DelayLightningEffects(); // 65

    TypeID GetTypeID() const { return m_typeID; }

    float distance(CGObject_C* secObj)
    {
        if (!secObj)
            return 0.0f;

        C3Vector a, b;
        secObj->GetPosition(a);
        this->GetPosition(b);

        float dx = b.X - a.X;
        float dy = b.Y - a.Y;
        float dz = b.Z - a.Z;
        return std::sqrt(dx * dx + dy * dy + dz * dz);
    }

private:
    uint32_t m_field4;        // 0x4
    uint32_t* m_data;         // 0x8
    uint32_t m_fieldC;        // 0xC
    uint32_t m_field10;       // 0x10
    TypeID m_typeID;          // 0x14
    uint32_t m_field18[46];   // 0x18
};

class CGUnit_C : public CGObject_C
{
public:
    int GetCreatureRank() const
    {
        typedef int(__thiscall* GetCreatureRank_t)(const CGObject_C* thisPtr);
        static const GetCreatureRank_t GetCreatureRankFn = reinterpret_cast<GetCreatureRank_t>(0x00718A00);

        return GetCreatureRankFn(this);
    }

    bool CanAssist(const CGUnit_C* unit, bool ignoreFlags) const
    {
        using CanAssist_t = bool(__thiscall*)(const CGUnit_C* thisPtr, const CGUnit_C* unit, bool ignoreFlags);
        static const CanAssist_t CanAssistFn = reinterpret_cast<CanAssist_t>(0x007293D0);

        return CanAssistFn(this, unit, ignoreFlags);
    }

    int UnitReaction(const CGUnit_C* unit) const
    {
        using UnitReaction_t = int(__thiscall*)(const CGUnit_C* thisPtr, const CGUnit_C* unit);
        static const UnitReaction_t UnitReactionFn = reinterpret_cast<UnitReaction_t>(0x007251C0);

        return UnitReactionFn(this, unit);
    }

    bool CanAttack(const CGUnit_C* unit) const
    {
        using CanAttack_t = bool(__thiscall*)(const CGUnit_C* thisPtr, const CGUnit_C* unit);
        static const CanAttack_t CanAttackFn = reinterpret_cast<CanAttack_t>(0x00729A70);

        return CanAttackFn(this, unit);
    }

    uint64_t GetGUID() const
    {
        return GetValue<uint64_t>(OBJECT_FIELD_GUID);
    }

};

class CGGameObject_C;

class CGGameObject_C : public CGObject_C
{
public:
    int CanUseNow() const
    {
        typedef bool(__thiscall* CanUseNow_t)(const CGGameObject_C* thisPtr);
        static const CanUseNow_t CanUseNowFn = reinterpret_cast<CanUseNow_t>(0x0070BA10); //set_name(0x0070BA10, "CGGameObject_C__CanUseNow");

        return CanUseNowFn(this);
    }
};

inline HWND GetGameWindow() { return *(HWND*)0x00D41620; }

namespace CGame {
    inline void __stdcall SetLastError(int code) { return ((decltype(&SetLastError))0x00771870)(code); }
}

inline int __stdcall gc_atoi(const char** str) { return ((decltype(&gc_atoi))0x76F190)(str); }
inline int gc_snprintf(char* buf, size_t size, const char* fmt, ...)
{
    if (!(buf && fmt)) {
        CGame::SetLastError(87);
        return 0;
    }

    va_list args;
    va_start(args, fmt);
    size_t size_ = size;
    char* buf_ = buf;
    const char* fmt_ = fmt;
    int result;
    _asm {
        pushad;
        pushfd;
        mov eax, args;
        mov esi, size_;
        mov edi, buf_;
        mov ecx, fmt_;
        mov ebx, 0x76F010;
        call ebx;
        mov result, eax;
        popad;
        popfd;
    }
    return result;
}

namespace RCString {
inline uint32_t __stdcall hash(const char* str) { return ((decltype(&hash))0x0076F640)(str); }
}

inline bool IsInWorld() { return *(char*)0x00BD0792; }

inline uintptr_t GetDbcTable(uint32_t dbIndex)
{
    for (uintptr_t tableBase = 0x006337D0; *(uint8_t*)tableBase != 0xC3; tableBase += 0x11) {
        uint32_t index = *(uint32_t*)(tableBase + 1);
        if (index == dbIndex) {
            uintptr_t tablePtr = *(uintptr_t*)(tableBase + 0xB) + 0x18;
            return tablePtr;
        }
    }
    return 0;
}

typedef int(__thiscall* ClientDb_GetLocalizedRow)(void* pThis, int index, void* rowBuffer);
inline auto GetLocalizedRow = (ClientDb_GetLocalizedRow)(0x004CFD20);

typedef int(__thiscall* ClientDb_GetRow)(void* pThis, int index);
inline auto GetRow = (ClientDb_GetRow)(0x004BB1C0);
// ObjectMgr
namespace ObjectMgr {

    inline int EnumObjects_internal(int(*func)(guid_t, void*), void* udata) { return ((decltype(&EnumObjects_internal))0x004D4B30)(func, udata); }

    using EnumVisibleObject_func_t = std::function<bool(guid_t guid)>;
    inline bool EnumObjects(EnumVisibleObject_func_t func)
    {
        struct Wrapper {
            static int foo(guid_t guid, void* udata)
            {
                EnumVisibleObject_func_t& func = *(EnumVisibleObject_func_t*)udata;
                return func(guid) ? 1 : 0;
            }
        };
        return EnumObjects_internal(&Wrapper::foo, (void*)&func);
    };

    inline Player* GetPlayer() { return ((decltype(&GetPlayer))0x004038F0)(); }
    inline Object* Get(guid_t guid, TypeMask flags) { return ((Object * (*)(guid_t, TypeMask))0x004D4DB0)(guid, flags); }
    inline void Guid2HexString(guid_t guid, char* buf) { return ((decltype(&Guid2HexString))0x0074D0D0)(guid, buf); }
    inline guid_t HexString2Guid(const char* str) { return ((decltype(&HexString2Guid))0x0074D120)(str); }
    inline guid_t GetGuidByUnitID(const char* unitId) { return ((decltype(&GetGuidByUnitID))0x0060C1C0)(unitId); }
    inline CGObject_C* GetObjectPtr(uint64_t objectGuid, uint32_t objectTypeMask) {
        typedef CGObject_C* (__cdecl* FuncPtr)(uint64_t, uint32_t, const char*, int);
        return ((FuncPtr)0x004D4DB0)(objectGuid, objectTypeMask, "", 0);
    }
    inline CGUnit_C* GetCGUnitPlayer() {
        long long lpguid = ((long long(__cdecl*)())(0x004D3790))();
        if (!(lpguid && ((int(__cdecl*)(long long, int))0x004D4DB0)(lpguid, 0x0010)))
            return nullptr;

        CGUnit_C* player = (CGUnit_C*)(ObjectMgr::GetObjectPtr(lpguid, TYPEMASK_UNIT));
        if (!player) {
            return nullptr;
        }
        return player;
    }
    inline guid_t String2Guid(const char* str)
    {
        if (!str) return 0;
        if (str[0] == '0' && (str[1] == 'x' || str[1] == 'X'))
            return HexString2Guid(str);
        else
            return GetGuidByUnitID(str);
    }

    inline Object* Get(const char* str, TypeMask flags) { return Get(String2Guid(str), flags); }

    inline int UnitRightClickByGuid(guid_t guid) { return ((decltype(&UnitRightClickByGuid))0x005277B0)(guid); }
    inline int UnitLeftClickByGuid(guid_t guid) { return ((decltype(&UnitLeftClickByGuid))0x005274F0)(guid); }
    inline void SetMouseoverByGuid(guid_t guid, guid_t prev) { return ((decltype(&SetMouseoverByGuid))0x0051F790)(guid, prev); }
    inline guid_t GetTargetGuid() { return *(guid_t*)0x00BD07B0; }
    inline guid_t GetPlayerGuid() { return *(guid_t*)0x004D3790; }

    const uintptr_t nameStore = 0x00C5D938 + 0x8;
    inline const char* UnitNameFromGuid(guid_t guid) {
        CGUnit_C* unit = (CGUnit_C*)(ObjectMgr::GetObjectPtr(guid, TYPEMASK_UNIT));
        if (!unit)
            return "UNKNOWN";

        return unit->GetObjectName();
    }
}

namespace Console {
enum CVarFlags : uint32_t {
    CVarFlags_ReadOnly = 0x4,
    CVarFlags_CheckTaint = 0x8,
    CVarFlags_HideFromUser = 0x40,
    CVarFlags_ReadOnlyForUser = 0x100,

};

struct CVar {
    using Handler_t = int(*)(CVar* cvar, const char* prevVal, const char* newVal, void* userData);

    uint32_t hash;
    uint32_t gap4[4];
    const char* name;
    uint32_t field18;
    CVarFlags flags;
    uint32_t field20;
    uint32_t field24;
    const char* vStr;
    uint32_t field2C[5];
    uint32_t vBool;
    uint32_t gap44[9];
    Handler_t handler;
    void* userData;
};
static_assert(sizeof(CVar) == 0x70);

inline CVar* RegisterCVar(const char* name, const char* desc, unsigned flags, const char* defaultVal, CVar::Handler_t callback, int a6, int a7, int a8, int a9) { return ((decltype(&RegisterCVar))0x00767FC0)(name, desc, flags, defaultVal, callback, a6, a7, a8, a9); };
inline CVar* GetCVar(const char* name) { return ((decltype(&GetCVar))0x00767460)(name); }
inline CVar* FindCVar(const char* name) { return ((decltype(&FindCVar))0x00767440)(name); }
inline char SetCVarValue(CVar* self, const char* value, int a3, int a4, int a5, int a6)
{
    return (((char(__thiscall*)(CVar*, const char*, int, int, int, int))0x007668C0))(self, value, a3, a4, a5, a6);
}
}

inline lua_State* GetLuaState() { return ((decltype(&GetLuaState))0x00817DB0)(); }
inline int GetLuaRefErrorHandler() { return *(int*)0x00AF576C; }

// CFrame
namespace CFrame {
inline int GetRefTable(Frame* frame) { return ((int(__thiscall*)(Frame*))0x00488380)(frame); }
inline Frame* Create(XMLObject* xml, Frame* parent, Status* status) { return ((decltype(&Create))0x00812FA0)(xml, parent, status); }
inline void SetFrameLevel(Frame* self, int level, int a3) { ((void(__thiscall*)(Frame*, int, int))0x004910A0)(self, level, a3); }
}

// FrameScript
namespace FrameScript {
struct Event {
    uint32_t hash;
    uint32_t gap4[4];
    const char* name;
    uint32_t gap18[12];
    uint32_t field48;
    uint32_t field4C;
    uint32_t field50;
};

struct EventList {
    size_t reserve;
    size_t size;
    Event** buf;
};

struct UnkContainer;

inline UnkContainer* GetUnkContainer() { return (UnkContainer*)0x00D3F7A8; }
inline Event* __fastcall FindEvent(UnkContainer* This, void* edx, const char* eventName) { return ((decltype(&FindEvent))0x004BC410)(This, edx, eventName); }
inline EventList* GetEventList() { return (EventList*)0x00D3F7D0; }
inline void FireEvent_inner(int eventId, lua_State* L, int nargs) { return ((decltype(&FireEvent_inner))0x0081AA00)(eventId, L, nargs); };
inline void vFireEvent(int eventId, const char* format, va_list args) { return ((decltype(&vFireEvent))0x0081AC90)(eventId, format, args); }

inline int GetEventIdByName(const char* eventName)
{
    EventList* eventList = GetEventList();
    if (eventList->size == 0)
        return -1;

    uint32_t hash = RCString::hash(eventName);
    for (size_t i = 0; i < eventList->size; i++) {
        Event* event = eventList->buf[i];
        if (event && event->hash == hash && (event->name == eventName || (strcmp(event->name, eventName) == 0)))
            return i;
    }
    return -1;
}

inline const char* GetEventNameById(unsigned idx)
{
    EventList* eventList = GetEventList();
    if (eventList->size == 0 || eventList->size < idx)
        return NULL;

    Event* event = eventList->buf[idx];
    return event ? event->name : NULL;
}

inline void FireEvent(const char* eventName, const char* format, ...)
{
    int eventId = GetEventIdByName(eventName);
    if (eventId == -1) return;

    va_list args;
    va_start(args, format);
    vFireEvent(eventId, format, args);
}
}

// NetClient
namespace NetClient {
inline void Login(const char* login, const char* password) { return ((decltype(&Login))0x004D8A30)(login, password); }
}

// LoginUI
namespace LoginUI {

#pragma pack(push, 1)
struct CharData {
    guid_t guid;
    char name[48];
    int map;
    int zone;
    int guildId;
    VecXYZ pos;
    int displayInfoId[23];
    int inventoryType[23];
    int enchantVisual[23];
    int petDisplayId;
    int petLevel;
    int petFamily;
    int flags;
    int charCustomizeFlags;
    char race;
    char class_;
    char gender;;
    char skin;
    char face;
    char hairStyle;
    char hairColor;
    char facialColor;
    char level;
    char firstLogin;
    char gap[6];
};
#pragma pack(pop)
static_assert(sizeof(CharData) == 0x188, "struct CharData corrupted");

struct CharVectorEntry {
    CharData data;
    // Note: It's not all fields
};

struct CharVector {
    int reserved;
    int size;
    CharVectorEntry* buf;
    int fieldC;
};

inline CharVector* GetChars() { return (CharVector*)0x00B6B238; }
inline void SelectCharacter(int idx)
{
    *(int*)0x00AC436C = idx;
    ((void(*)())0x004E3CD0)();
}

inline void EnterWorld(int idx)
{
    //lasthardware action = current timestamp
    *(int*)0x00B499A4 = *(int*)0x00B1D618;
    *(int*)0x00AC436C = idx;
    ((void(*)())0x004D9BD0)();
}

}

// WorldFrame & Camera
struct CameraVtbl;

struct Camera {
    CameraVtbl* vmt;
    uint32_t field4;
    VecXYZ pos;
    uint32_t gap14[11];
    float fovInRadians;
    float aspect;
};

inline Camera* GetActiveCamera() { return ((decltype(&GetActiveCamera))0x004F5960)(); }
inline WorldFrame* GetWorldFrame() { return *(WorldFrame**)0x00B7436C; }
inline int __fastcall WorldFrame_3Dto2D(WorldFrame* This, void* edx, VecXYZ* pos3d, VecXYZ* pos2d, uint32_t* flags) { return ((decltype(&WorldFrame_3Dto2D))0x004F6D20)(This, edx, pos3d, pos2d, flags); }
inline void WorldFrame_PercToScreenPos(float x, float y, float* resX, float* resY)
{
    float screenHeightAptitude = *(float*)0x00AC0CBC;
    float someVal = *(float*)0x00AC0CB4;
    *resX = (x * (screenHeightAptitude * 1024.f)) / someVal;
    *resY = (y * (screenHeightAptitude * 1024.f)) / someVal;
}

// XML
struct __declspec(novtable) XMLObject {
    uint32_t gap0[0x38 / 4];

    inline XMLObject(int a1, const char* parentName) { ((XMLObject * (__thiscall*)(XMLObject*, int, const char*))0x00814AD0)(this, a1, parentName); }
    inline void setValue(const char* key, const char* value) { ((void(__thiscall*)(XMLObject*, const char*, const char*))0x814C40)(this, key, value); }
};

// Lua
#define lua_pop(L,n)		lua_settop(L, -(n)-1)
#define lua_isfunction(L,n)	(lua_type(L, (n)) == LUA_TFUNCTION)
#define lua_istable(L,n)	(lua_type(L, (n)) == LUA_TTABLE)
#define lua_islightuserdata(L,n)	(lua_type(L, (n)) == LUA_TLIGHTUSERDATA)
#define lua_isuserdata(L,n) (lua_type(L,n) == LUA_TLIGHTUSERDATA) || (lua_type(L, n) == LUA_TUSERDATA)
#define lua_isnil(L,n)		(lua_type(L, (n)) == LUA_TNIL)
#define lua_isstring(L,n)	(lua_type(L, (n)) == LUA_TSTRING)
#define lua_isnumber(L,n)	(lua_type(L, (n)) == LUA_TNUMBER)
#define lua_isboolean(L,n)	(lua_type(L, (n)) == LUA_TBOOLEAN)
#define lua_isthread(L,n)	(lua_type(L, (n)) == LUA_TTHREAD)
#define lua_isnone(L,n)		(lua_type(L, (n)) == LUA_TNONE)
#define lua_isnoneornil(L, n)	(lua_type(L, (n)) <= 0)
#define lua_pushcfunction(L, f) lua_pushcclosure(L, f, 0);
#define lua_setglobal(L,s)	lua_setfield(L, LUA_GLOBALSINDEX, (s))
#define lua_getglobal(L,s)	lua_getfield(L, LUA_GLOBALSINDEX, (s))
#define lua_tostring(L,i)	lua_tolstring(L, (i), NULL)
#define lua_newtable(L) lua_createtable(L, 0, 0)
#define luaL_checkstring(L, i) luaL_checklstring(L, i, NULL)

#define LUA_TNONE		(-1)
#define LUA_TNIL		0
#define LUA_TBOOLEAN		1
#define LUA_TLIGHTUSERDATA	2
#define LUA_TNUMBER		3
#define LUA_TSTRING		4
#define LUA_TTABLE		5
#define LUA_TFUNCTION		6
#define LUA_TUSERDATA		7
#define LUA_TTHREAD		8

#define LUA_REGISTRYINDEX	(-10000)
#define LUA_ENVIRONINDEX	(-10001)
#define LUA_GLOBALSINDEX	(-10002)
#define lua_upvalueindex(i)	(LUA_GLOBALSINDEX-(i))


using lua_CFunction = int(*)(lua_State*);
typedef struct luaL_Reg {
    const char* name;
    lua_CFunction func;
} luaL_Reg;

inline void luaL_checktype(lua_State* L, int idx, int t) { return ((decltype(&luaL_checktype))0x0084F960)(L, idx, t); }
inline const char* luaL_checklstring(lua_State* L, int idx, size_t* len) { return ((decltype(&luaL_checklstring))0x0084F9F0)(L, idx, len); }
inline lua_Number luaL_checknumber(lua_State* L, int idx) { return ((decltype(&luaL_checknumber))0x84FAB0)(L, idx); }
inline void* lua_touserdata(lua_State* L, int idx) { return ((decltype(&lua_touserdata))0x0084E1C0)(L, idx); }
inline double lua_tonumber(lua_State* L, int n_param) { return ((double(__cdecl*)(lua_State*, int))0x0084E030)(L, n_param); }
inline char* lua_tostringnew(lua_State* state, int n_param){ return ((char* (__cdecl*)(lua_State*, int, int))0x0084E0E0)(state, n_param, 0); }
inline void lua_pushstring(lua_State* L, const char* str) { return ((decltype(&lua_pushstring))0x0084E350)(L, str); }
inline void lua_pushboolean(lua_State* L, bool b) { return ((decltype(&lua_pushboolean))0x0084E4D0)(L, b); }
inline void lua_pushvalue(lua_State* L, int idx) { return ((decltype(&lua_pushvalue))0x0084DE50)(L, idx); }
inline void lua_pushnumber(lua_State* L, lua_Number v) { return ((decltype(&lua_pushnumber))0x0084E2A0)(L, v); }
inline void lua_pushcclosure(lua_State* L, lua_CFunction func, int c) { return ((decltype(&lua_pushcclosure))0x0084E400)(L, func, c); }
inline void lua_pushnil(lua_State* L) { return ((decltype(&lua_pushnil))0x0084E280)(L); }
inline void lua_rawseti(lua_State* L, int idx, int pos) { return ((decltype(&lua_rawseti))0x0084EA00)(L, idx, pos); }
inline void lua_rawgeti(lua_State* L, int idx, int pos) { return ((decltype(&lua_rawgeti))0x0084E670)(L, idx, pos); }
inline void lua_rawset(lua_State* L, int idx) { return ((decltype(&lua_rawset))0x0084E970)(L, idx); }
inline void lua_rawget(lua_State* L, int idx) { return ((decltype(&lua_rawget))0x0084E600)(L, idx); }
inline void lua_setfield(lua_State* L, int idx, const char* str) { return ((decltype(&lua_setfield))0x0084E900)(L, idx, str); }
inline void lua_getfield(lua_State* L, int idx, const char* str) { return ((decltype(&lua_getfield))0x0084E590)(L, idx, str); }
inline int lua_next(lua_State* L, int idx) { return ((decltype(&lua_next))0x0084EF50)(L, idx); }
inline void lua_insert(lua_State* L, int idx) { return ((decltype(&lua_insert))0x0084DCC0)(L, idx); }
inline int lua_gettop(lua_State* L) { return ((decltype(&lua_gettop))0x0084DBD0)(L); }
inline void lua_settop(lua_State* L, int idx) { return ((decltype(&lua_settop))0x0084DBF0)(L, idx); }
inline int lua_objlen(lua_State* L, int idx) { return ((decltype(&lua_objlen))0x0084E150)(L, idx); }
inline int lua_type(lua_State* L, int idx) { return ((decltype(&lua_type))0x0084DEB0)(L, idx); }
inline int lua_pcall(lua_State* L, int argn, int retn, int eh) { return ((decltype(&lua_pcall))0x0084EC50)(L, argn, retn, eh); }
inline int lua_GetParamValue(lua_State* L, int idx, int default_) { return ((decltype(&lua_GetParamValue))0x00815500)(L, idx, default_); }
inline void lua_createtable(lua_State* L, int narr, int nrec) { return ((decltype(&lua_createtable))0x0084E6E0)(L, narr, nrec); }
inline void* lua_newuserdata(lua_State* L, size_t size) { return ((decltype(&lua_newuserdata))0x0084F0F0)(L, size); }
inline int lua_setmetatable(lua_State* L, int idx) { return ((decltype(&lua_setmetatable))0x0084EA90)(L, idx); }

inline void lua_wipe(lua_State* L, int idx)
{
    if (idx < 0) idx = lua_gettop(L) - (idx + 1);
    lua_pushnil(L); // nil
    while (lua_next(L, idx)) { // key, value
        lua_pop(L, 1); // key
        lua_pushnil(L); // key, nil
        lua_rawset(L, idx); //
        lua_pushnil(L); // key, nil
    }
}

inline void lua_pushguid(lua_State* L, guid_t guid)
{
    char buf[24];
    ObjectMgr::Guid2HexString(guid, buf);
    lua_pushstring(L, buf);
}

inline Frame* lua_toframe(lua_State* L, int idx)
{
    __asm {
        mov esi, L;
        push idx;
        mov eax, 0x004A81B0;
        call eax;
        ret;
    }
}

inline Frame* lua_toframe_silent(lua_State* L, int idx)
{
    lua_rawgeti(L, idx, 0);
    Frame* frame = (Frame*)lua_touserdata(L, -1);
    lua_pop(L, 1);
    return frame;
}

inline void lua_pushframe(lua_State* L, Frame* frame)
{
    lua_rawgeti(L, LUA_REGISTRYINDEX, CFrame::GetRefTable(frame));
}


struct MockCStatus {
    void** vtable;
};

static bool CheckIfBindingOrHeaderExists(const char* upperName, const char* upperHeader) {
    lua_State* L = GetLuaState();
    if (!L) return false;

    // Check Lua global: BINDING_NAME_<NAME>
    if (upperName && upperName[0]) {
        lua_getfield(L, LUA_GLOBALSINDEX, ("BINDING_NAME_" + std::string(upperName)).c_str());
        if (!lua_isnil(L, -1)) {
            lua_pop(L, 1);
            return true;
        }
        lua_pop(L, 1);
    }

    // Check Lua global: BINDING_HEADER_<HEADER>
    if (upperHeader && upperHeader[0]) {
        lua_getfield(L, LUA_GLOBALSINDEX, ("BINDING_HEADER_" + std::string(upperHeader)).c_str());
        if (!lua_isnil(L, -1)) {
            lua_pop(L, 1);
            return true;
        }
        lua_pop(L, 1);
    }

    // Native XML table check
    void* root = *reinterpret_cast<void**>(0x00BEADD8);
    void** hashTable = reinterpret_cast<void**>(root) + 3;

    using SStrHashHTFn = void* (__thiscall*)(void*, const char*);
    auto SStrHashHT = reinterpret_cast<SStrHashHTFn>(0x0055F4D0); // Double-check this address

    // Check for binding name in native table
    if (upperName && upperName[0] && SStrHashHT(hashTable, upperName)) {
        return true;
    }

    // Check for "HEADER_<HEADER>" in native table
    if (upperHeader && upperHeader[0]) {
        std::string fullHeaderKey = "HEADER_" + std::string(upperHeader);
        if (SStrHashHT(hashTable, fullHeaderKey.c_str())) {
            return true;
        }
    }

    return false;
}

//based on and credits to: https://github.com/leoaviana/ConsoleXP/blob/38cb2a1ece253b017b3e554a2146033e95054ae2/src/ConsoleXP/Game.cpp#L727
static void RegisterLuaBinding(const char* bindingName, const char* bindingText, const char* bindingHeaderName, const char* bindingHeaderText, const char* luaScript)
{
    lua_State* L = GetLuaState();
    if (!L) return;

    std::string upperHeader(bindingHeaderName);
    std::transform(upperHeader.begin(), upperHeader.end(), upperHeader.begin(), ::toupper);

    std::string upperName(bindingName);
    std::transform(upperName.begin(), upperName.end(), upperName.begin(), ::toupper);

    if (CheckIfBindingOrHeaderExists(upperName.c_str(), upperHeader.c_str())) {
        return;
    }

    if (bindingHeaderName && bindingHeaderName[0]) {
        lua_pushstring(L, bindingHeaderText);
        lua_setfield(L, LUA_GLOBALSINDEX, ("BINDING_HEADER_" + upperHeader).c_str());
    }

    if (bindingName && bindingName[0]) {
        lua_pushstring(L, bindingText);
        lua_setfield(L, LUA_GLOBALSINDEX, ("BINDING_NAME_" + upperName).c_str());
    }

    // Build XML node
    XMLObject node(0, "Bindings");
    node.setValue("name", upperName.c_str());
    node.setValue("header", bindingHeaderName ? bindingHeaderName : "");

    // Inject script text
    *reinterpret_cast<char**>(reinterpret_cast<uint8_t*>(&node) + 0x18) = const_cast<char*>(luaScript);

    // Call native loader
    void* thispointer = *reinterpret_cast<void**>(reinterpret_cast<uint8_t*>(0x00BEADD8));
    using LoadBindFn = void(__thiscall*)(void*, void*, XMLObject*, MockCStatus*);
    auto LdBinding = reinterpret_cast<LoadBindFn>(0x00564470);

    static void* dummyVtable[4] = {};
    MockCStatus status;
    status.vtable = dummyVtable;

    char bindsName[] = "AWESOME_KEYBIND";
    LdBinding(thispointer, bindsName, &node, &status);
}
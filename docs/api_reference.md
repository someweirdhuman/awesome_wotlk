[C_NamePlates](#c_nameplate) - [Unit](#unit) - [Inventory](#inventory) - [Misc](#misc)

# C_NamePlate
Backported C-Lua interfaces from retail

## C_NamePlate.GetNamePlateForUnit`API`
Arguments: **unitId** `string`

Returns: **namePlate**`frame`

Get nameplates by unitId
```lua
frame = C_NamePlate.GetNamePlateForUnit("target")
```

## C_NamePlate.GetNamePlates`API`
Arguments: `none`

Returns: **namePlateList**`table`

Get all visible nameplates
```lua
for _, nameplate in pairs(C_NamePlate.GetNamePlates()) do
  -- something
end
```

## C_NamePlate.GetNamePlateByGUID`API`
Arguments: `none`

Returns: **namePlateList**`table`

Get nameplate from UnitGUID for example from combat log
```lua
local nameplate = C_NamePlate.GetNamePlateByGUID(destGUID)
```

## C_NamePlate.GetNamePlateTokenByGUID`API`
Arguments: `none`

Returns: **namePlateList**`table`

Get nameplate token from UnitGUID for example from combat log
```lua
local token = C_NamePlate.GetNamePlateTokenByGUID(destGUID)
local frame = C_NamePlate.GetNamePlateForUnit(token)
```

## NAME_PLATE_CREATED`Event`
Parameters: **namePlateBase**`frame`

Fires when nameplate was created

## NAME_PLATE_UNIT_ADDED`Event`
Parameters: **unitId**`string`

Notifies that a new nameplate appeared

## NAME_PLATE_UNIT_REMOVED`Event`
Parameters: **unitId**`string`

Notifies that a nameplate will be hidden

## NAME_PLATE_OWNER_CHANGED`Event`
Parameters: **unitId**`string`

Fires when nameplate owner changed (workaround for [this issue](https://github.com/FrostAtom/awesome_wotlk/blob/main/src/AwesomeWotlkLib/NamePlates.cpp#L170))

## nameplateDistance`CVar`
Arguments: **distance**`number`

Default: **41**

Sets the display distance of nameplates in yards

## nameplateStacking`CVar`
Arguments: **enabled**`boolean`

Default: **0**

Enables or disables nameplateStacking feature

## nameplateXSpace`CVar`
Arguments: **width**`number`

Default: **130**

Sets the effective width of a nameplate used in the collision/stacking calculation.

## nameplateYSpace`CVar`  
Arguments: **height**`number`

Default: **20**

Sets the effective height of a nameplate used in the stacking collision calculation. 

## nameplateUpperBorder`CVar`  
Arguments: **offset**`number`

Default: **30**

Defines the vertical offset from the top of the screen where nameplates stop stacking upward.

## nameplateOriginPos`CVar`
Arguments: **offset**`number`

Default: **20**

Offset used to push nameplate bit higher than its default position

## nameplateSpeedRaise`CVar` 
Arguments: **speed**`number`

Default: **1**

Speed at which nameplates move **upward** during stacking resolution. 

## nameplateSpeedReset`CVar` 
Arguments: **speed**`number`

Default: **1**

Speed at which nameplates **reset** during stacking resolution. 

## nameplateSpeedLower`CVar` 
Arguments: **speed**`number`

Default: **1**

Speed at which nameplates move **downward** during stacking resolution. 

## nameplateHitboxHeight`CVar` 
Arguments: **height**`number`

Default: **0**

Height of a clickable nameplate hitbox, addons may override or break this, reload or disable/enable nameplates afterwards.<br>
Use 0 to disable this and use default values.

## nameplateHitboxWidth`CVar` 
Arguments: **width**`number`

Default: **0**

Width of a clickable nameplate hitbox, addons may override or break this, reload or disable/enable nameplates afterwards.<br>
Use 0 to disable this and use default values.

## interactionMode`CVar` 
Arguments: **mode**`bool`

Default: **1**

Toggles behaviour of interaction keybind, or macro. <br>
If set to **1**, interaction is limited to entities located in front of the player within the angle defined by the `interactionAngle` CVar and within 20 yards.<br>
If set to **0**, interaction will occur with the nearest entity within 20 yards of the player, regardless of its direction.

## interactionAngle`CVar` 
Arguments: **angle**`number`

Default: **60**

The size of the cone-shaped area in front of the player (measured in degrees) within which a mob or entity must be located to be eligible for interaction. <br>
This is only used if `interactionMode` is set to 1, which is the default.

## nameplateStackFriendly`CVar` 
Arguments: **toggle**`bool`

Default: **1**

Toggles if friendly nameplates are stacking or overlapping. <br>
If set to **0** then it overlaps<br>
If set to **1** it stacks. 

## nameplateStackFriendlyMode`CVar` 
Arguments: **mode**`number`

Default: **1**

Changes how friendliness of mobs is decided. <br>
If set to **0** a UnitReaction("player", "nameplate%") > 2 is used.<br>
If set to **1** a parsing of healthbar color is used, same as weakaura did it.

# Unit

## UnitIsControlled`API`
Arguments: **unitId**`string`

Returns: **isControlled**`bool`

Returns true if unit being hard control

## UnitIsDisarmed`API`
Arguments: **unitId**`string`

Returns: **isDisarmed**`bool`

Returns true if unit is disarmed

## UnitIsSilenced`API`
Arguments: **unitId**`string`

Returns: **isSilenced**`bool`

Returns true if unit is silenced

## UnitOccupations`API`
Arguments: **unitID**`string`

Returns: **npcFlags**`number`

Returns [npcFlags bitmask](https://github.com/someweirdhuman/awesome_wotlk/blob/7ab28cea999256d4c769b8a1e335a7d93c5cac32/src/AwesomeWotlkLib/UnitAPI.cpp#L37) if passed valid unitID else returns nothing

## UnitOwner`API`
Arguments: **unitID**`string`

Returns: **ownerName**`string`, **ownerGuid**`string`

Returns ownerName and ownerGuid if passed valid unitID else returns nothing

## UnitTokenFromGUID`API`
Arguments: **GUID**`string`

Returns: **UnitToken**`string`

Returns UnitToken if passed valid GUID else returns nothing

# Inventory

## GetInventoryItemTransmog`API`
Arguments: **unitId**`string`, **slot**`number`

Returns: **itemId**`number`, **enchantId**`number`

Returns info about item transmogrification

# Spell

## GetSpellBaseCooldown`API`
Arguments: **spellId**`string`

Returns: **cdMs**`number`, **gcdMs**`number`

Returns cooldown and global cooldown in milliseconds if passed valid spellId else returns nothing

# Item

## GetItemInfoInstant`API`
Arguments: **itemId/itemName/itemHyperlink**`string`

Returns: **itemID**`number`, **itemType**`string`, **itemSubType**`string`, **itemEquipLoc**`string`, **icon**`string`, **classID**`number`, **subclassID**`number`,

Returns id, type, sub-type, equipment slot, icon, class id, and sub-class id if passed valid argument else returns nothing

# Misc

## FlashWindow`API`
Arguments: `none`

Returns: `none`

Starts flashing of game window icon in taskbar

## IsWindowFocused`API`
Arguments: `none`

Returns: `bool`

Returns 1 if game window is focused, overtwice nil

## FocusWindow`API`
Arguments: `none`

Returns: `none`

Raise game window

## CopyToClipboard`API`
Arguments: **text**`string`

Returns: `none`

Copies text to clipboard

## cameraFov`CVar`
Parameters: **value**`number`

Default: **100**

Сhanges the camera view area (fisheye effect), in range **1**-**200**

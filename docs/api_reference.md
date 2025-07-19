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

## NAME_PLATE_UNIT_CHANGED`Event`
Parameters: **unitId**`string`

Fires when nameplate owner changed(?)

## nameplateDistance`CVar`
Arguments: **distance**`number`

Default: **41**

Sets the display distance of nameplates in yards

## nameplateStacking`CVar`
Arguments: **enabled**`boolean`

Default: **0**

Enables or disables nameplateStacking feature

### nameplateXSpace`CVar`
Arguments: **width**`number`

Default: **130**

Sets the effective width of a nameplate used in the collision/stacking calculation.

### nameplateYSpace`CVar`  
Arguments: **height**`number`

Default: **20**

Sets the effective height of a nameplate used in the stacking collision calculation. 

### nameplateUpperBorder`CVar`  
Arguments: **offset**`number`

Default: **30**

Defines the vertical offset from the top of the screen where nameplates stop stacking upward.

### nameplateOriginPos`CVar`
Arguments: **offset**`number`

Default: **20**

Offset used to push nameplate bit higher than its default position

### nameplateSpeedRaise`CVar` 
Arguments: **speed**`number`

Default: **1**

Speed at which nameplates move **upward** during stacking resolution. 

### nameplateSpeedReset`CVar` 
Arguments: **speed**`number`

Default: **1**

Speed at which nameplates **reset** during stacking resolution. 

### nameplateSpeedLower`CVar` 
Arguments: **speed**`number`

Default: **1**

Speed at which nameplates move **downward** during stacking resolution. 

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

# Inventory

## GetInventoryItemTransmog`API`
Arguments: **unitId**`string`, **slot**`number`

Returns: **itemId**`number`, **enchantId**`number`

Returns info about item transmogrification

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

## UnitOccupations`API`
Arguments: **unitID**`string`

Returns: **npcFlags**`number`

Returns npcFlags if passed valid unitID else returns nothing

## UnitOwner`API`
Arguments: **unitID**`string`

Returns: **ownerName**`string`, **ownerGuid**`string`, 

Returns ownerName and ownerGuid if passed valid unitID else returns nothing

## UnitTokenFromGUID`API`
Arguments: **GUID**`string`

Returns: **UnitToken**`string`

Returns UnitToken if passed valid GUID else returns nothing

## cameraFov`CVar`
Parameters: **value**`number`

Default: **100**

Сhanges the camera view area (fisheye effect), in range **1**-**200**

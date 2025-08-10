# Awesome WotLK
## World of Warcraft 3.3.5a 12340 improvements library
### Fork of https://github.com/FrostAtom/awesome_wotlk/

## <b> [Details](#details) - [Installation](#installation) - [Docs](https://github.com/someweirdhuman/awesome_wotlk/blob/main/docs/api_reference.md) - [For suggestions](#for-suggestions) - [3rd party libraries](#3rd-party-libraries)

___
## Details
> - BugFix: clipboard issue when non-english text becomes "???"
> - Auto Login (Through cmdline/shortcuts, Usage: `Wow.exe -login "LOGIN" -password "PASSWORD" -realmlist "REALMLIST" -realmname "REALMNAME" `)
> - Changing cameras FOV
> - Improved nameplates sorting
> - Backported `cursor` macro conditional
> - Implemented `playerlocation` macro conditional
> - Fixed client bug where client couldnt cast second ability after changing form or stance, now `/cast battle stance /cast charge` works in a single click
> - New API:<br>
    - C_NamePlate.GetNamePlates<br>
    - C_NamePlate.GetNamePlateForUnit<br>
    - C_NamePlate.GetNamePlateByGUID<br>
    - C_NamePlate.GetNamePlateTokenByGUID<br>
    - UnitIsControlled<br>
    - UnitIsDisarmed<br>
    - UnitIsSilenced<br>
    - GetInventoryItemTransmog<br>
    - FlashWindow<br>
    - IsWindowFocused<br>
    - FocusWindow<br>
    - CopyToClipboard<br>
    - UnitOccupations<br>
    - UnitOwner<br>
    - UnitTokenFromGUID<br>
    - GetSpellBaseCooldown<br>
    - GetItemInfoInstant<br>
    - QueueInteract
> - New events:<br>
    - NAME_PLATE_CREATED<br>
    - NAME_PLATE_UNIT_ADDED<br>
    - NAME_PLATE_UNIT_REMOVED<br>
    - NAME_PLATE_OWNER_CHANGED
> - New CVars:<br>
    - nameplateDistance<br>
    - nameplateStacking<br>
    - nameplateXSpace<br>
    - nameplateYSpace<br>
    - nameplateUpperBorder<br>
    - nameplateOriginPos<br>
    - nameplateSpeedRaise<br>
    - nameplateSpeedReset<br>
    - nameplateSpeedLower<br>
    - nameplateHitboxHeight<br>
    - nameplateHitboxWidth<br>
    - nameplateFriendlyHitboxHeight<br>
    - nameplateFriendlyHitboxWidth<br>
    - interactionMode<br>
    - interactionAngle<br>
    - nameplateStackFriendly<br>
    - nameplateStackFriendlyMode<br>
    - nameplateMaxRaiseDistance<br>
    - nameplateExtendWorldFrameHeight<br>
    - nameplateUpperBorderOnlyBoss<br>
    - enableStancePatch<br>
    - cameraIndirectVisibility<br>
    - cameraIndirectAlpha
> - New Interaction Keybind:<br>
    - It loots mobs, skins mobs, interacts with near object like veins, chairs, doors, etc, mailboxes, etc.<br>
    - You can keybind this in options menu like any other keybind (Requires Interaction Addon, bundled in release rar)<br>
    - You can macro this using /interact, interact should also support some modifiers like @mouseover - /interact [@mouseover], blizzard rules apply
> - New Nameplate Stacking:<br>
    - enable stacking nameplates by doing /console nameplateStacking 1 - /reload recommended after both disabling or enabling <br>
    - remember to delete this weakaura https://wago.io/AQdGXNEBH if you are using it and restarting client before using this feature <br>
    - see docs for more cvar details <br>
    - everything configurable in /awesome (addon) <br>
See [Docs](https://github.com/someweirdhuman/awesome_wotlk/blob/main/docs/api_reference.md) for details

**Recommended to use AwesomeCVar addon and use ingame command /awesome to configure all cvars**

**AwesomeCVar Addon**<br>
![AwesomeCVar Preview](https://raw.githubusercontent.com/someweirdhuman/awesome_wotlk/refs/heads/main/docs/assets/preview.png)

## Installation
1) Download latest [release](https://github.com/someweirdhuman/awesome_wotlk/releases)
2) Unpack files to root game folder
3) Launch `AwesomeWotlkPatch.exe`, you should get a message (or drag wow.exe on top of AwesomeWotlkPatch.exe)
4) To update just download and replace dll

## 3rd party libraries
- [microsoft-Detours](https://github.com/microsoft/Detours) - [license](https://github.com/microsoft/Detours/blob/6782fe6e6ab11ae34ae66182aa5a73b5fdbcd839/LICENSE.md)

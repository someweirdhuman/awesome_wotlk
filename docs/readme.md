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
    - UnitTokenFromGUID
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
    - nameplateHitboxWidth
    - interactionMode
    - interactionAngle
> - New Interaction Keybind:<br>
    - It loots mobs, skins mobs, interacts with near object like veins, chairs, doors, etc, mailboxes, etc.<br>
    - You can keybind this in options menu like any other keybind (Requires Interaction Addon, bundled in release rar)
    - You can macro this using /interact, interact should also support some modifiers like @mouseover - /interact [@mouseover], blizzard rules apply
> - New Nameplate Stacking:<br>
    - based on https://wago.io/AQdGXNEBH <br>
    - enable stacking nameplates by doing /console nameplateStacking 1 - /reload recommended after both disabling or enabling <br>
    - also remember to delete this weakaura https://wago.io/AQdGXNEBH and restarting client before using this feature <br>
    - see docs for more cvar details <br>
See [Docs](https://github.com/someweirdhuman/awesome_wotlk/blob/main/docs/api_reference.md) for details

## Installation
1) Download latest [release](https://github.com/someweirdhuman/awesome_wotlk/releases)
2) Unpack files to root game folder
3) Launch `AwesomeWotlkPatch.exe`, you should get a message (or drag wow.exe on top of AwesomeWotlkPatch.exe)
4) To update just download and replace dll

## 3rd party libraries
- [microsoft-Detours](https://github.com/microsoft/Detours) - [license](https://github.com/microsoft/Detours/blob/6782fe6e6ab11ae34ae66182aa5a73b5fdbcd839/LICENSE.md)

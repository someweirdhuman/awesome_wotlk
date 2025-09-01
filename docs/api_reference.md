[C_NamePlates](#c_nameplate) - [C_VoiceChat](#c_voicechat) - [Unit](#unit) - [Inventory](#inventory) - [Misc](#misc)

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
Arguments: **enabled**`bool`

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

## nameplateFriendlyHitboxHeight`CVar` 
Arguments: **height**`number`

Default: **0**

Height of a clickable **Friendly** nameplate hitbox, addons may override or break this, reload or disable/enable nameplates afterwards.<br>
Use 0 to disable this and use default values.
Affected by choice of `nameplateStackFriendlyMode`, which has unintuitive name.

## nameplateFriendlyHitboxWidth`CVar` 
Arguments: **width**`number`

Default: **0**

Width of a clickable **Friendly** nameplate hitbox, addons may override or break this, reload or disable/enable nameplates afterwards.<br>
Use 0 to disable this and use default values.

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
If set to **0** a UnitReaction("player", "nameplate%") >= 5 + CanAttack check for reaction 4 is used.<br>
If set to **1** a parsing of healthbar color is used, same as weakaura did it.

## nameplateMaxRaiseDistance`CVar` 
Arguments: **height**`number`

Default: **200**

Sets maximum height nameplate can go up, before deciding to go down, or staying there. 

## nameplateExtendWorldFrameHeight`CVar` 
Arguments: **enabled**`bool`

Default: **0**

When enabled, this extends the height of the WorldFrame, allowing nameplates that would normally be out of view to remain visible.
Note: This may interfere with some UI elements or addons that rely on the original WorldFrame height. It’s recommended to use a WeakAura or an addon to toggle this setting only during raids or boss encounters.

## nameplateUpperBorderOnlyBoss`CVar` 
Arguments: **enabled**`bool`

Default: **0**

When enabled, only nameplates of boss creatures will stick to top of the screen, all other nameplates will overflow.

# C_VoiceChat
Windows SAPI–backed Text-to-Speech backport from retail

## C_VoiceChat.GetTtsVoices`API`
Arguments: `none`  
Returns: **voiceList**`table` → `{ { voiceID = number, name = string }, ... }`

Returns all locally available TTS voices.

```lua
for _, v in ipairs(C_VoiceChat.GetTtsVoices()) do
  print(v.voiceID, v.name)
end
````

## C\_VoiceChat.GetRemoteTtsVoices`API`

Arguments: `none`
Returns: **voiceList**`table`

Same as `GetTtsVoices()`.

## C\_VoiceChat.SpeakText`API`

Arguments:

* **voiceID**`number`
* **text**`string`
* **destination**`number` *(optional, default=1)*
* **rate**`number` *(optional)*
* **volume**`number` *(optional)*

Returns: **utteranceID**`number`

Speaks a text asynchronously.

* `destination = 1` → speak immediately (FIFO)
* `destination = 4` → accepted, no special handling (async)

```lua
C_VoiceChat.SpeakText(1, "Hello World", 1, 0, 100)
```

## C\_VoiceChat.StopSpeakingText`API`

Arguments: `none`
Returns: `none`

Stops all queued or currently playing utterances.

## C\_TTSSettings.GetSpeechRate`API`

Arguments: `none`
Returns: **rate**`number` \[-10..10]

## C\_TTSSettings.GetSpeechVolume`API`

Arguments: `none`
Returns: **volume**`number` \[0..100]

## C\_TTSSettings.GetSpeechVoiceID`API`

Arguments: `none`
Returns: **voiceID**`number`

## C\_TTSSettings.GetVoiceOptionName`API`

Arguments: `none`
Returns: **voiceName**`string`

## C\_TTSSettings.SetDefaultSettings`API`

Arguments: `none`
Returns: `none`

Resets to defaults: voice=1 (if available), rate=0, volume=100.

## C\_TTSSettings.SetSpeechRate`API`

Arguments: **rate**`number` \[-10..10]
Returns: `none`

## C\_TTSSettings.SetSpeechVolume`API`

Arguments: **volume**`number` \[0..100]
Returns: `none`

## C\_TTSSettings.SetVoiceOption`API`

Arguments: **voiceID**`number`
Returns: `none`

## C\_TTSSettings.SetVoiceOptionByName`API`

Arguments: **voiceName**`string`
Returns: `none`

## C\_TTSSettings.RefreshVoices`API`

Arguments: `none`
Returns: `none`

Refreshes the voice list.
Fires `VOICE_CHAT_TTS_VOICES_UPDATE` if the list has changed.

# C_VoiceChat Events

## VOICE\_CHAT\_TTS\_PLAYBACK\_STARTED`Event`

Parameters: **numConsumers**`number`, **utteranceID**`number`, **durationMS**`number`, **destination**`number`

Fired when SAPI starts playback.
`durationMS` is always **0**.

## VOICE\_CHAT\_TTS\_PLAYBACK\_FINISHED`Event`

Parameters: **numConsumers**`number`, **utteranceID**`number`, **destination**`number`

Fired when SAPI finishes playback.

## VOICE\_CHAT\_TTS\_PLAYBACK\_FAILED`Event`

Parameters: **status**`string`, **utteranceID**`number`, **destination**`number`

Fired if `SpeakText()` or setup fails (e.g. no voice/device available).

## VOICE\_CHAT\_TTS\_SPEAK\_TEXT\_UPDATE`Event`

Parameters: **status**`string`, **utteranceID**`number`

Unused placeholder.

## VOICE\_CHAT\_TTS\_VOICES\_UPDATE`Event`

Parameters: `none`

Fired when the enumerated voice list changes.

# C_VoiceChat CVars

## ttsVoice`CVar`

Arguments: **voiceID**`number`
Default: **1**

Sets the active voice.

## ttsSpeed`CVar`

Arguments: **rate**`number` \[-10..10]
Default: **0**

Controls the speech rate.

## ttsVolume`CVar`

Arguments: **volume**`number` \[0..100]
Default: **100**

Controls the speech volume.# C_VoiceChat
Windows SAPI–backed Text-to-Speech backport from retail

## C_VoiceChat.GetTtsVoices`API`
Arguments: `none`  
Returns: **voiceList**`table` → `{ { voiceID = number, name = string }, ... }`

Returns all locally available TTS voices.

```lua
for _, v in ipairs(C_VoiceChat.GetTtsVoices()) do
  print(v.voiceID, v.name)
end
````

## C\_VoiceChat.GetRemoteTtsVoices`API`

Arguments: `none`
Returns: **voiceList**`table`

Same as `GetTtsVoices()`.

## C\_VoiceChat.SpeakText`API`

Arguments:

* **voiceID**`number`
* **text**`string`
* **destination**`number` *(optional, default=1)*
* **rate**`number` *(optional)*
* **volume**`number` *(optional)*

Returns: **utteranceID**`number`

Speaks a text asynchronously.

* `destination = 1` → speak immediately (FIFO)
* `destination = 4` → accepted, no special handling (async)

```lua
C_VoiceChat.SpeakText(1, "Hello World", 1, 0, 100)
```

## C\_VoiceChat.StopSpeakingText`API`

Arguments: `none`
Returns: `none`

Stops all queued or currently playing utterances.

## C\_TTSSettings.GetSpeechRate`API`

Arguments: `none`
Returns: **rate**`number` \[-10..10]

## C\_TTSSettings.GetSpeechVolume`API`

Arguments: `none`
Returns: **volume**`number` \[0..100]

## C\_TTSSettings.GetSpeechVoiceID`API`

Arguments: `none`
Returns: **voiceID**`number`

## C\_TTSSettings.GetVoiceOptionName`API`

Arguments: `none`
Returns: **voiceName**`string`

## C\_TTSSettings.SetDefaultSettings`API`

Arguments: `none`
Returns: `none`

Resets to defaults: voice=1 (if available), rate=0, volume=100.

## C\_TTSSettings.SetSpeechRate`API`

Arguments: **rate**`number` \[-10..10]
Returns: `none`

## C\_TTSSettings.SetSpeechVolume`API`

Arguments: **volume**`number` \[0..100]
Returns: `none`

## C\_TTSSettings.SetVoiceOption`API`

Arguments: **voiceID**`number`
Returns: `none`

## C\_TTSSettings.SetVoiceOptionByName`API`

Arguments: **voiceName**`string`
Returns: `none`

## C\_TTSSettings.RefreshVoices`API`

Arguments: `none`
Returns: `none`

Refreshes the voice list.
Fires `VOICE_CHAT_TTS_VOICES_UPDATE` if the list has changed.

# VoiceChat TTS Events

## VOICE\_CHAT\_TTS\_PLAYBACK\_STARTED`Event`

Parameters: **numConsumers**`number`, **utteranceID**`number`, **durationMS**`number`, **destination**`number`

Fired when SAPI starts playback.
`durationMS` is always **0**.

## VOICE\_CHAT\_TTS\_PLAYBACK\_FINISHED`Event`

Parameters: **numConsumers**`number`, **utteranceID**`number`, **destination**`number`

Fired when SAPI finishes playback.

## VOICE\_CHAT\_TTS\_PLAYBACK\_FAILED`Event`

Parameters: **status**`string`, **utteranceID**`number`, **destination**`number`

Fired if `SpeakText()` or setup fails (e.g. no voice/device available).

## VOICE\_CHAT\_TTS\_SPEAK\_TEXT\_UPDATE`Event`

Parameters: **status**`string`, **utteranceID**`number`

Unused placeholder.

## VOICE\_CHAT\_TTS\_VOICES\_UPDATE`Event`

Parameters: `none`

Fired when the enumerated voice list changes.

# VoiceChat TTS CVars

## ttsVoice`CVar`

Arguments: **voiceID**`number`
Default: **1**

Sets the active voice.

## ttsSpeed`CVar`

Arguments: **rate**`number` \[-10..10]
Default: **0**

Controls the speech rate.

## ttsVolume`CVar`

Arguments: **volume**`number` \[0..100]
Default: **100**

Controls the speech volume.

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

## enableStancePatch`CVar`
Arguments: **enabled**`bool`

Default: **0**

Enables patch that allows you to swap stance or form and cast next ability, thats not on gcd, in a single click.

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

## cameraIndirectVisibility`CVar` 
Arguments: **enabled**`number`

Default: **0**

Toggles behaviour of camera when obstructed by objects in the world. <br>
If set to **0**, it uses default client behaviour.
If set to **1**, it allows your camera to move freely through some world objects without being blocked.

## cameraIndirectAlpha`CVar` 
Arguments: **alpha**`number`

Default: **0.6**

Controls the transparency level of objects between the camera and the player character when cameraIndirectVisibility is enabled. <br>
Limited to [0.6 - 1] range.

## cameraIndirectOffset`CVar` 
Arguments: **offset**`number`

Default: **10**

Not implemented yet. <br>

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

## Cursor`macro`

Backported `cursor` macro conditional for quick-casting aoe spells at cursors position<br>
/cast [@cursor] Blizzard<br>
/cast [target=cursor] Flare

## Playerlocation`macro`

Implemented `playerlocation` macro conditional for quick-casting aoe spells at players (you) location<br>
/cast [@playerlocation] Blizzard<br>
/cast [target=playerlocation] Flare

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

-- File: enUS.lua
-- Language: English (US)
local addonName, AwesomeCVar = ...

if not AwesomeCVar.L then
    AwesomeCVar.L = {}
end

local L = AwesomeCVar.L

if GetLocale() == "enUS" then
    -- General
    L.ADDON_NAME = "AwesomeCVar"
    L.ADDON_NAME_SHORT = "Awesome CVar"
    L.MAIN_FRAME_TITLE = "Awesome CVar Manager"
    L.RESET_TO = "Reset to %s"

    -- Popups
    L.RELOAD_POPUP_TITLE = "Reload UI Required"
    L.RELOAD_POPUP_TEXT = "One or more of the changes you have made require a ReloadUI to take effect."
    L.RESET_POPUP_TITLE = "Confirm Default Reset"
    L.RESET_POPUP_TEXT = "Are you sure you want to reset all values back to their defaults?"

    -- Chat Messages
    L.MSG_LOADED = "Awesome CVar loaded! Type /awesome to open the manager."
    L.MSG_FRAME_RESET = "Frame position has been reset to the center."
    L.MSG_SET_VALUE = "Set %s to %s."
    L.MSG_FRAME_CREATE_ERROR = "AwesomeCVar frame could not be created!"
    L.MSG_UNKNOWN_COMMAND = "Unknown command. Type /awesome help for available commands."
    L.MSG_HELP_HEADER = "Awesome CVar Commands:"
    L.MSG_HELP_TOGGLE = "/awesome - Toggle the CVar manager"
    L.MSG_HELP_SHOW = "/awesome show - Show the CVar manager"
    L.MSG_HELP_HIDE = "/awesome hide - Hide the CVar manager"
    L.MSG_HELP_RESET = "/awesome reset - Reset frame position to center"
    L.MSG_HELP_HELP = "/awesome help - Show this help message"

    -- CVar Categories
    L.CATEGORY_CAMERA = "Camera"
    L.CATEGORY_NAMEPLATES = "Nameplates"
    L.CATEGORY_INTERACTION = "Interaction"
    L.CATEGORY_OTHER = "Other"

    -- CVar Labels & Descriptions
    L.CVAR_LABEL_CAMERA_FOV = "Camera FoV"
    L.CVAR_LABEL_ENABLE_STACKING = "Enable Nameplate Stacking"
    L.CVAR_LABEL_STACK_FRIENDLY = "Stack Friendly Nameplates |cffff0000(Reload Required)|r"
    L.CVAR_LABEL_FRIENDLY_DETECT_MODE = "Friendly Detection Mode |cffff0000(Reload Required)|r"
    L.CVAR_LABEL_STACKING_FUNCTION = "Stacking Function |cffff0000(Reload Required)|r"
    L.CVAR_LABEL_NAMEPLATE_DISTANCE = "Nameplate Distance"
    L.CVAR_LABEL_MAX_RAISE_DISTANCE = "Max Raise Distance"
    L.CVAR_LABEL_X_SPACE = "Nameplate X Space"
    L.CVAR_LABEL_Y_SPACE = "Nameplate Y Space"
    L.CVAR_LABEL_UPPER_BORDER = "Nameplate Upper Border Offset"
    L.CVAR_LABEL_ORIGIN_POS = "Nameplate Origin Offset"
    L.CVAR_LABEL_SPEED_RAISE = "Nameplate Speed Raise"
    L.CVAR_LABEL_SPEED_RESET = "Nameplate Speed Reset"
    L.CVAR_LABEL_SPEED_LOWER = "Nameplate Speed Lower"
    L.CVAR_LABEL_HITBOX_HEIGHT = "Nameplate Hitbox Height"
    L.CVAR_LABEL_HITBOX_WIDTH = "Nameplate Hitbox Width"
    L.CVAR_LABEL_FRIENDLY_HITBOX_HEIGHT = "FRIENDLY Nameplate Hitbox Height"
    L.CVAR_LABEL_FRIENDLY_HITBOX_WIDTH = "FRIENDLY Nameplate Hitbox Width"
    L.CVAR_LABEL_INTERACTION_MODE = "Interaction Mode"
    L.CVAR_LABEL_INTERACTION_ANGLE = "Interaction Cone Angle"
    L.CVAR_LABEL_SPELL_PROJECTION_MODE = "Spell Projection Mode"
	L.CVAR_LABEL_SPELL_PROJECTION_MAX_RANGE = "Spell Projection Max Range"
	L.CVAR_LABEL_SPELL_PROJECTION_HORIZONTAL_BIAS = "Spell Projection Horizontal Bias"
	L.CVAR_LABEL_CAMERA_INDIRECT_VISIBILITY = "Camera Indirect Visibility"
	L.CVAR_LABEL_CAMERA_INDIRECT_ALPHA = "Camera Indirect Alpha"
	L.CVAR_LABEL_CAMERA_INDIRECT_OFFSET = "Camera Indirect Offset"

	L.DESC_CAMERA_SPEED = ""
	L.DESC_ZERO_TO_DISABLE = "0 = DISABLED"
    L.DESC_HITBOX_DISABLED = "0 = DISABLED, don't use without a nameplate addon"
    L.DESC_SMOOTH_FUNCTION_ONLY = "(only works with the Smooth Function)"

    -- CVar Mode Options
    L.MODE_LABEL_REACTION_API = "Reaction API"
    L.MODE_LABEL_COLOR_PARSING = "Color Parsing"
    L.MODE_LABEL_LEGACY = "Legacy (Weakaura-style)"
    L.MODE_LABEL_SMOOTH = "Smooth Function"
    L.MODE_LABEL_PLAYER_RADIUS = "Player Radius 20yd"
    L.MODE_LABEL_CONE_ANGLE = "Cone Angle (dg) within 20yd"
    L.MODE_LABEL_PROJECTION_DEFAULT = "Default spell projection (3.3.5)"
    L.MODE_LABEL_PROJECTION_CUSTOM = "Custom spell projection (Classic)"
	L.MODE_LABEL_CAMERA_DEFAULT = "Default Camera"
	L.MODE_LABEL_CAMERA_CUSTOM = "Custom Camera"
end
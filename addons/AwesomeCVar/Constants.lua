-- File: Constants.lua
-- Holds all static definitions for the addon.

local addonName, ACVar = ...
local L = ACVar.L or {} -- Get the locale table loaded previously

-- This table holds constants used throughout the addon.
ACVar.CONSTANTS = {
    ADDON_NAME = L.ADDON_NAME,
    COLORS = {
        SUCCESS = "|cff00ff00",
        HIGHLIGHT = "|cffffd100",
        VALUE = "|cff00ccff",
        ERROR = "|cffff0000",
        RESET = "|r",
        TAB_ACTIVE = {1, 1, 0},
        TAB_INACTIVE = {0.8, 0.8, 0.8},
        DESC_TEXT = {0.6, 0.6, 0.6}
    },
    FRAME = {
        MAIN_WIDTH = 768,
        MAIN_HEIGHT = 580,
        POPUP_WIDTH = 350,
        POPUP_HEIGHT = 120,
        BUTTON_WIDTH = 100,
        BUTTON_HEIGHT = 25,
        TAB_HEIGHT = 25
    }
}

-- This table defines every CVar control that will appear in the UI.
ACVar.CVARS = {
    [L.CATEGORY_CAMERA] = {
        { name = "cameraFov", label = L.CVAR_LABEL_CAMERA_FOV, type = "slider", min = 30, max = 150, default = 90 },
    },
    [L.CATEGORY_NAMEPLATES] = {
        { name = "nameplateStacking", label = L.CVAR_LABEL_ENABLE_STACKING, type = "toggle", min = 0, max = 1 },
        { name = "nameplateStackFriendly", label = L.CVAR_LABEL_STACK_FRIENDLY, type = "toggle", min = 0, max = 1, reloadRequired = true },
        { name = "nameplateStackFriendlyMode", label = L.CVAR_LABEL_FRIENDLY_DETECT_MODE, type = "mode", modes = { {value = 0, label = L.MODE_LABEL_REACTION_API}, {value = 1, label = L.MODE_LABEL_COLOR_PARSING} }, reloadRequired = true },
        { name = "nameplateStackFunction", label = L.CVAR_LABEL_STACKING_FUNCTION, type = "mode", modes = { {value = 0, label = L.MODE_LABEL_LEGACY}, {value = 1, label = L.MODE_LABEL_SMOOTH} }, reloadRequired = true },
        { name = "nameplateDistance", label = L.CVAR_LABEL_NAMEPLATE_DISTANCE, type = "slider", min = 1, max = 200, step = 1, default = 40 },
        { name = "nameplateMaxRaiseDistance", label = L.CVAR_LABEL_MAX_RAISE_DISTANCE, desc = L.DESC_SMOOTH_FUNCTION_ONLY, type = "slider", min = 0, max = 500, step = 1, default = 200 },
        { name = "nameplateXSpace", label = L.CVAR_LABEL_X_SPACE, type = "slider", min = 5, max = 200, step = 1, default = 10 },
        { name = "nameplateYSpace", label = L.CVAR_LABEL_Y_SPACE, type = "slider", min = 5, max = 75, step = 1, default = 15 },
        { name = "nameplateUpperBorder", label = L.CVAR_LABEL_UPPER_BORDER, type = "slider", min = 0, max = 100, step = 1, default = 50 },
        { name = "nameplateOriginPos", label = L.CVAR_LABEL_ORIGIN_POS, type = "slider", min = 0, max = 60, step = 1, default = 30 },
        { name = "nameplateSpeedRaise", label = L.CVAR_LABEL_SPEED_RAISE, type = "slider", min = 0, max = 5, step = 0.1, default = 1.0 },
        { name = "nameplateSpeedReset", label = L.CVAR_LABEL_SPEED_RESET, type = "slider", min = 0, max = 5, step = 0.1, default = 1.0 },
        { name = "nameplateSpeedLower", label = L.CVAR_LABEL_SPEED_LOWER, type = "slider", min = 0, max = 5, step = 0.1, default = 1.0 },
        { name = "nameplateHitboxHeight", label = L.CVAR_LABEL_HITBOX_HEIGHT, desc = L.DESC_HITBOX_DISABLED, type = "slider", min = 0, max = 50, step = 1, default = 0 },
        { name = "nameplateHitboxWidth", label = L.CVAR_LABEL_HITBOX_WIDTH, desc = L.DESC_HITBOX_DISABLED, type = "slider", min = 0, max = 200, step = 1, default = 0 },
        { name = "nameplateFriendlyHitboxHeight", label = L.CVAR_LABEL_FRIENDLY_HITBOX_HEIGHT, desc = L.DESC_HITBOX_DISABLED, type = "slider", min = 0, max = 50, step = 1, default = 0 },
        { name = "nameplateFriendlyHitboxWidth", label = L.CVAR_LABEL_FRIENDLY_HITBOX_WIDTH, desc = L.DESC_HITBOX_DISABLED, type = "slider", min = 0, max = 200, step = 1, default = 0 },
    },
    [L.CATEGORY_INTERACTION] = {
        { name = "interactionMode", label = L.CVAR_LABEL_INTERACTION_MODE, type = "mode", modes = { {value = 0, label = L.MODE_LABEL_PLAYER_RADIUS}, {value = 1, label = L.MODE_LABEL_CONE_ANGLE} } },
        { name = "interactionAngle", label = L.CVAR_LABEL_INTERACTION_ANGLE, type = "slider", min = 1, max = 360, step = 1, default = 90 },
    },
    [L.CATEGORY_OTHER] = {
        { name = "spellProjectionMode", label = L.CVAR_LABEL_SPELL_PRJECTION_MODE, type = "mode", modes = { {value = 0, label = L.MODE_LABEL_PROJECTION_DEFAULT}, {value = 1, label = L.MODE_LABEL_PROJECTION_CUSTOM} } },
    },
}
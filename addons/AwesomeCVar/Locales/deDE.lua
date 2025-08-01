-- File: deDE.lua
-- Language: German
local addonName, AwesomeCVar = ...

if not AwesomeCVar.L then
    AwesomeCVar.L = {}
end

local L = AwesomeCVar.L

if GetLocale() == "deDE" then
    -- General
    L.ADDON_NAME = "AwesomeCVar"
    L.ADDON_NAME_SHORT = "Awesome CVar"
    L.MAIN_FRAME_TITLE = "Awesome CVar-Manager"
    L.RESET_TO = "Zurücksetzen auf %s"

    -- Popups
    L.RELOAD_POPUP_TITLE = "Neuladen der UI erforderlich"
    L.RELOAD_POPUP_TEXT = "Eine oder mehrere der von Ihnen vorgenommenen Änderungen erfordern ein Neuladen der Benutzeroberfläche (/reload), um wirksam zu werden."
    L.RESET_POPUP_TITLE = "Standardeinstellungen bestätigen"
    L.RESET_POPUP_TEXT = "Sind Sie sicher, dass Sie alle Werte auf ihre Standardeinstellungen zurücksetzen möchten?"

    -- Chat Messages
    L.MSG_LOADED = "Awesome CVar geladen! Geben Sie /awesome ein, um den Manager zu öffnen."
    L.MSG_FRAME_RESET = "Fensterposition wurde auf die Mitte zurückgesetzt."
    L.MSG_SET_VALUE = "%s auf %s gesetzt."
    L.MSG_FRAME_CREATE_ERROR = "AwesomeCVar-Fenster konnte nicht erstellt werden!"
    L.MSG_UNKNOWN_COMMAND = "Unbekannter Befehl. Geben Sie /awesome help für verfügbare Befehle ein."
    L.MSG_HELP_HEADER = "Awesome CVar-Befehle:"
    L.MSG_HELP_TOGGLE = "/awesome - Schaltet den CVar-Manager um"
    L.MSG_HELP_SHOW = "/awesome show - Zeigt den CVar-Manager an"
    L.MSG_HELP_HIDE = "/awesome hide - Verbirgt den CVar-Manager"
    L.MSG_HELP_RESET = "/awesome reset - Setzt die Fensterposition auf die Mitte zurück"
    L.MSG_HELP_HELP = "/awesome help - Zeigt diese Hilfenachricht an"

    -- CVar Categories
    L.CATEGORY_CAMERA = "Kamera"
    L.CATEGORY_NAMEPLATES = "Namensplaketten"
    L.CATEGORY_INTERACTION = "Interaktion"

    -- CVar Labels & Descriptions
    L.CVAR_LABEL_CAMERA_FOV = "Kamera-Sichtfeld (FoV)"
    L.CVAR_LABEL_ENABLE_STACKING = "Stapeln von Namensplaketten aktivieren"
    L.CVAR_LABEL_STACK_FRIENDLY = "Freundliche Namensplaketten stapeln |cffff0000(Neuladen erf.)|r"
    L.CVAR_LABEL_FRIENDLY_DETECT_MODE = "Erkennungsmodus für Freunde |cffff0000(Neuladen erf.)|r"
    L.CVAR_LABEL_STACKING_FUNCTION = "Stapelfunktion |cffff0000(Neuladen erf.)|r"
    L.CVAR_LABEL_NAMEPLATE_DISTANCE = "Distanz der Namensplaketten"
    L.CVAR_LABEL_MAX_RAISE_DISTANCE = "Max. Anhebedistanz"
    L.CVAR_LABEL_X_SPACE = "X-Abstand der Namensplaketten"
    L.CVAR_LABEL_Y_SPACE = "Y-Abstand der Namensplaketten"
    L.CVAR_LABEL_UPPER_BORDER = "Oberer Rand-Offset der Namensplaketten"
    L.CVAR_LABEL_ORIGIN_POS = "Ursprungs-Offset der Namensplaketten"
    L.CVAR_LABEL_SPEED_RAISE = "Anhebegeschwindigkeit der Namensplaketten"
    L.CVAR_LABEL_SPEED_RESET = "Rücksetzgeschwindigkeit der Namensplaketten"
    L.CVAR_LABEL_SPEED_LOWER = "Absenkgeschwindigkeit der Namensplaketten"
    L.CVAR_LABEL_HITBOX_HEIGHT = "Hitbox-Höhe der Namensplaketten"
    L.CVAR_LABEL_HITBOX_WIDTH = "Hitbox-Breite der Namensplaketten"
    L.CVAR_LABEL_FRIENDLY_HITBOX_HEIGHT = "FREUNDLICHE Namensplaketten Hitbox-Höhe"
    L.CVAR_LABEL_FRIENDLY_HITBOX_WIDTH = "FREUNDLICHE Namensplaketten Hitbox-Breite"
    L.CVAR_LABEL_INTERACTION_MODE = "Interaktionsmodus"
    L.CVAR_LABEL_INTERACTION_ANGLE = "Interaktionskegelwinkel"

    L.DESC_HITBOX_DISABLED = "0 = DEAKTIVIERT, nicht ohne ein Namensplaketten-Addon verwenden"
    L.DESC_SMOOTH_FUNCTION_ONLY = "(funktioniert nur mit der 'Smooth'-Funktion)"

    -- CVar Mode Options
    L.MODE_LABEL_REACTION_API = "Reaktions-API"
    L.MODE_LABEL_COLOR_PARSING = "Farbanalyse"
    L.MODE_LABEL_LEGACY = "Legacy (Weakaura-Stil)"
    L.MODE_LABEL_SMOOTH = "Smooth-Funktion"
    L.MODE_LABEL_PLAYER_RADIUS = "Spielerradius 20yd"
    L.MODE_LABEL_CONE_ANGLE = "Kegelwinkel (Grad) innerhalb von 20yd"
end
-- File: frFR.lua
-- Language: French
local addonName, AwesomeCVar = ...

if not AwesomeCVar.L then
    AwesomeCVar.L = {}
end

local L = AwesomeCVar.L

if GetLocale() == "frFR" then
    -- General
    L.ADDON_NAME = "AwesomeCVar"
    L.ADDON_NAME_SHORT = "Awesome CVar"
    L.MAIN_FRAME_TITLE = "Gestionnaire Awesome CVar"
    L.RESET_TO = "Réinitialiser à %s"

    -- Popups
    L.RELOAD_POPUP_TITLE = "Rechargement de l'interface requis"
    L.RELOAD_POPUP_TEXT = "Un ou plusieurs des changements que vous avez effectués nécessitent un rechargement de l'interface (/reload) pour prendre effet."
    L.RESET_POPUP_TITLE = "Confirmer la réinitialisation par défaut"
    L.RESET_POPUP_TEXT = "Êtes-vous sûr de vouloir rétablir toutes les valeurs par défaut ?"

    -- Chat Messages
    L.MSG_LOADED = "Awesome CVar chargé ! Tapez /awesome pour ouvrir le gestionnaire."
    L.MSG_FRAME_RESET = "La position de la fenêtre a été réinitialisée au centre."
    L.MSG_SET_VALUE = "%s défini sur %s."
    L.MSG_FRAME_CREATE_ERROR = "La fenêtre AwesomeCVar n'a pas pu être créée !"
    L.MSG_UNKNOWN_COMMAND = "Commande inconnue. Tapez /awesome help pour les commandes disponibles."
    L.MSG_HELP_HEADER = "Commandes Awesome CVar :"
    L.MSG_HELP_TOGGLE = "/awesome - Affiche/cache le gestionnaire CVar"
    L.MSG_HELP_SHOW = "/awesome show - Affiche le gestionnaire CVar"
    L.MSG_HELP_HIDE = "/awesome hide - Cache le gestionnaire CVar"
    L.MSG_HELP_RESET = "/awesome reset - Réinitialise la position de la fenêtre au centre"
    L.MSG_HELP_HELP = "/awesome help - Affiche ce message d'aide"

    -- CVar Categories
    L.CATEGORY_CAMERA = "Caméra"
    L.CATEGORY_NAMEPLATES = "Barres de noms"
    L.CATEGORY_INTERACTION = "Interaction"
    L.CATEGORY_OTHER = "Autre"

    -- CVar Labels & Descriptions
    L.CVAR_LABEL_CAMERA_FOV = "Champ de vision (FoV)"
    L.CVAR_LABEL_ENABLE_STACKING = "Activer l'empilement des barres de noms"
    L.CVAR_LABEL_STACK_FRIENDLY = "Empiler les barres de noms amicales |cffff0000(Recharg. requis)|r"
    L.CVAR_LABEL_FRIENDLY_DETECT_MODE = "Mode de détection amical |cffff0000(Recharg. requis)|r"
    L.CVAR_LABEL_STACKING_FUNCTION = "Fonction d'empilement |cffff0000(Recharg. requis)|r"
    L.CVAR_LABEL_NAMEPLATE_DISTANCE = "Distance des barres de noms"
    L.CVAR_LABEL_MAX_RAISE_DISTANCE = "Distance d'élévation max"
    L.CVAR_LABEL_X_SPACE = "Espace X des barres de noms"
    L.CVAR_LABEL_Y_SPACE = "Espace Y des barres de noms"
    L.CVAR_LABEL_UPPER_BORDER = "Décalage bordure supérieure"
    L.CVAR_LABEL_ORIGIN_POS = "Décalage d'origine"
    L.CVAR_LABEL_SPEED_RAISE = "Vitesse d'élévation"
    L.CVAR_LABEL_SPEED_RESET = "Vitesse de réinitialisation"
    L.CVAR_LABEL_SPEED_LOWER = "Vitesse de descente"
    L.CVAR_LABEL_HITBOX_HEIGHT = "Hauteur de la hitbox"
    L.CVAR_LABEL_HITBOX_WIDTH = "Largeur de la hitbox"
    L.CVAR_LABEL_FRIENDLY_HITBOX_HEIGHT = "Hauteur de la hitbox AMICALE"
    L.CVAR_LABEL_FRIENDLY_HITBOX_WIDTH = "Largeur de la hitbox AMICALE"
    L.CVAR_LABEL_INTERACTION_MODE = "Mode d'interaction"
    L.CVAR_LABEL_INTERACTION_ANGLE = "Angle du cône d'interaction"
    L.CVAR_LABEL_SPELL_PROJECTION_MODE = "Mode de projection des sorts"
	L.CVAR_LABEL_SPELL_PROJECTION_MAX_RANGE = "Portée maximale de projection de sort"
	L.CVAR_LABEL_SPELL_PROJECTION_HORIZONTAL_BIAS = "Biais horizontal de la projection de sort"
	L.CVAR_LABEL_CAMERA_INDIRECT_VISIBILITY = "Visibilité indirecte de la caméra"
	L.CVAR_LABEL_CAMERA_INDIRECT_ALPHA = "Alpha indirect de la caméra"
	L.CVAR_LABEL_CAMERA_INDIRECT_OFFSET = "Décalage indirect de la caméra"

	L.DESC_CAMERA_SPEED = ""
	L.DESC_ZERO_TO_DISABLE = "0 = DÉSACTIVÉ"
    L.DESC_HITBOX_DISABLED = "0 = DÉSACTIVÉ, ne pas utiliser sans un addon de barres de noms"
    L.DESC_SMOOTH_FUNCTION_ONLY = "(fonctionne uniquement avec la Fonction Douce)"

    -- CVar Mode Options
    L.MODE_LABEL_REACTION_API = "API de Réaction"
    L.MODE_LABEL_COLOR_PARSING = "Analyse de couleur"
    L.MODE_LABEL_LEGACY = "Legacy (style Weakaura)"
    L.MODE_LABEL_SMOOTH = "Fonction Douce"
    L.MODE_LABEL_PLAYER_RADIUS = "Rayon du joueur 20m"
    L.MODE_LABEL_CONE_ANGLE = "Angle du cône (dg) à moins de 20m"
    L.MODE_LABEL_PROJECTION_DEFAULT = "Projection des sorts par défaut (3.3.5)"
    L.MODE_LABEL_PROJECTION_CUSTOM = "Projection des sorts personnalisée (Classic)"
	L.MODE_LABEL_CAMERA_DEFAULT = "Caméra par défaut"
	L.MODE_LABEL_CAMERA_CUSTOM = "Caméra personnalisée"
end
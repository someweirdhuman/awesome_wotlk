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
    L.CATEGORY_TEXT_TO_SPEECH = "Text to Speech"
    L.CATEGORY_INTERACTION = "Interaction"
    L.CATEGORY_OTHER = "Autres"

    -- CVar Labels & Descriptions
    L.CVAR_LABEL_TTS_VOICE = "TTS Voice"
    L.CVAR_LABEL_TTS_VOLUME = "TTS Volume"
    L.CVAR_LABEL_TTS_SPEED = "TTS Rate"
    L.CVAR_LABEL_CAMERA_FOV = "Champ de vision (FoV)"
    L.CVAR_LABEL_ENABLE_STACKING = "Activer l'empilement des barres de noms"
    L.CVAR_LABEL_STACK_FRIENDLY = "Empiler les barres de noms amicales |cffff0000(Recharg. requis)|r"
    L.CVAR_LABEL_FRIENDLY_DETECT_MODE = "Mode de détection amical |cffff0000(Recharg. requis)|r"
    L.CVAR_LABEL_NAMEPLATE_DISTANCE = "Distance des barres de noms"
    L.CVAR_LABEL_MAX_RAISE_DISTANCE = "Distance d'élévation max"
    L.CVAR_LABEL_X_SPACE = "Espace X des barres de noms"
    L.CVAR_LABEL_Y_SPACE = "Espace Y des barres de noms"
    L.CVAR_LABEL_UPPER_BORDER = "Décalage bordure supérieure"
    L.CVAR_LABEL_UPPER_BORDER_ONLY_BOSS = "Autoriser UNIQUEMENT les boss à coller à la bordure supérieure de l'écran"
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
    L.CVAR_LABEL_EXTEND_WORLD_FRAME_HEIGHT = "Étendre la hauteur du World Frame"
    L.CVAR_ENABLE_STANCE_PATCH = "Activer le patch de changement de posture/forme"
    L.CVAR_ENABLE_SHOW_PLAYER = "Activer l'affichage du modèle du personnage du joueur"
    L.CVAR_LABEL_CAMERA_INDIRECT_VISIBILITY = "Visibilité indirecte de la caméra"
    L.CVAR_LABEL_CAMERA_INDIRECT_ALPHA = "Alpha indirect de la caméra"
    L.CVAR_LABEL_CAMERA_INDIRECT_OFFSET = "Décalage indirect de la caméra"
    L.CVAR_LABEL_CAMERA_DISTANCE_MAX = "Distance de la caméra"

    L.DESC_HITBOX_DISABLED = "0 = DÉSACTIVÉ, ne pas utiliser sans un addon de barres de noms"
    L.DESC_EXTEND_WORLD_FRAME_HEIGHT = "Prolonge les barres de nom hors de la vue vers la bordure supérieure ; peut casser certains éléments de l’interface. NÉCESSITE l’activation de l’empilement des barres de nom."
    L.DESC_STANCE_PATCH = "Permet de changer de posture/forme et de lancer une capacité via macro en un seul clic. Ne fonctionne pas sur Warmane."
    L.DESC_CAMERA_INDIRECT_VISIBILITY = "Permet à votre caméra de se déplacer librement à travers certains objets du monde sans être bloquée."
    L.DESC_CAMERA_INDIRECT_ALPHA = "Définit la transparence des objets entre la caméra et le joueur."
    L.DESC_CAMERA_DISTANCE_MAX = "Définit la distance maximale à laquelle vous pouvez zoomer vers l'extérieur."

    -- CVar Mode Options
    L.MODE_LABEL_REACTION_API = "API de Réaction"
    L.MODE_LABEL_COLOR_PARSING = "Analyse de couleur"
    L.MODE_LABEL_PLAYER_RADIUS = "Rayon du joueur 20m"
    L.MODE_LABEL_CONE_ANGLE = "Angle du cône (dg) à moins de 20m"
end
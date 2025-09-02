-- File: esMX.lua
-- Language: Spanish (Mexico)
local addonName, AwesomeCVar = ...

if not AwesomeCVar.L then
    AwesomeCVar.L = {}
end

local L = AwesomeCVar.L

if GetLocale() == "esMX" then
    -- General
    L.ADDON_NAME = "AwesomeCVar"
    L.ADDON_NAME_SHORT = "Awesome CVar"
    L.MAIN_FRAME_TITLE = "Administrador de Awesome CVar"
    L.RESET_TO = "Restablecer a %s"

    -- Popups
    L.RELOAD_POPUP_TITLE = "Se requiere recargar la IU"
    L.RELOAD_POPUP_TEXT = "Uno o más de los cambios que ha realizado requieren recargar la interfaz (/reload) para que surtan efecto."
    L.RESET_POPUP_TITLE = "Confirmar reinicio a predeterminados"
    L.RESET_POPUP_TEXT = "¿Estás seguro de que quieres restablecer todos los valores a sus valores predeterminados?"

    -- Chat Messages
    L.MSG_LOADED = "¡Awesome CVar cargado! Escribe /awesome para abrir el administrador."
    L.MSG_FRAME_RESET = "La posición del marco se ha restablecido al centro."
    L.MSG_SET_VALUE = "Se estableció %s a %s."
    L.MSG_FRAME_CREATE_ERROR = "¡No se pudo crear el marco de AwesomeCVar!"
    L.MSG_UNKNOWN_COMMAND = "Comando desconocido. Escribe /awesome help para ver los comandos disponibles."
    L.MSG_HELP_HEADER = "Comandos de Awesome CVar:"
    L.MSG_HELP_TOGGLE = "/awesome - Activa/desactiva el administrador de CVar"
    L.MSG_HELP_SHOW = "/awesome show - Muestra el administrador de CVar"
    L.MSG_HELP_HIDE = "/awesome hide - Oculta el administrador de CVar"
    L.MSG_HELP_RESET = "/awesome reset - Restablece la posición del marco al centro"
    L.MSG_HELP_HELP = "/awesome help - Muestra este mensaje de ayuda"

    -- CVar Categories
    L.CATEGORY_CAMERA = "Cámara"
    L.CATEGORY_NAMEPLATES = "Placas de nombre"
    L.CATEGORY_TEXT_TO_SPEECH = "Text to Speech"
    L.CATEGORY_INTERACTION = "Interacción"
    L.CATEGORY_OTHER = "Otros"

    -- CVar Labels & Descriptions
    L.CVAR_LABEL_TTS_VOICE = "TTS Voice"
    L.CVAR_LABEL_TTS_VOLUME = "TTS Volume"
    L.CVAR_LABEL_TTS_SPEED = "TTS Rate"
    L.CVAR_LABEL_CAMERA_FOV = "Campo de visión (FoV) de la cámara"
    L.CVAR_LABEL_ENABLE_STACKING = "Habilitar apilamiento de placas de nombre"
    L.CVAR_LABEL_STACK_FRIENDLY = "Apilar placas de nombre aliadas |cffff0000(Req. recarga)|r"
    L.CVAR_LABEL_FRIENDLY_DETECT_MODE = "Modo de detección de aliados |cffff0000(Req. recarga)|r"
    L.CVAR_LABEL_NAMEPLATE_DISTANCE = "Distancia de placas de nombre"
    L.CVAR_LABEL_MAX_RAISE_DISTANCE = "Distancia máxima de elevación"
    L.CVAR_LABEL_X_SPACE = "Espacio X de placas de nombre"
    L.CVAR_LABEL_Y_SPACE = "Espacio Y de placas de nombre"
    L.CVAR_LABEL_UPPER_BORDER = "Desplazamiento del borde superior"
    L.CVAR_LABEL_UPPER_BORDER_ONLY_BOSS = "Permitir SOLO a los jefes pegarse al borde superior de la pantalla"
    L.CVAR_LABEL_ORIGIN_POS = "Desplazamiento de origen"
    L.CVAR_LABEL_SPEED_RAISE = "Velocidad de elevación"
    L.CVAR_LABEL_SPEED_RESET = "Velocidad de reinicio"
    L.CVAR_LABEL_SPEED_LOWER = "Velocidad de descenso"
    L.CVAR_LABEL_HITBOX_HEIGHT = "Altura del hitbox"
    L.CVAR_LABEL_HITBOX_WIDTH = "Ancho del hitbox"
    L.CVAR_LABEL_FRIENDLY_HITBOX_HEIGHT = "Altura del hitbox ALIADO"
    L.CVAR_LABEL_FRIENDLY_HITBOX_WIDTH = "Ancho del hitbox ALIADO"
    L.CVAR_LABEL_INTERACTION_MODE = "Modo de interacción"
    L.CVAR_LABEL_INTERACTION_ANGLE = "Ángulo del cono de interacción"
    L.CVAR_LABEL_EXTEND_WORLD_FRAME_HEIGHT = "Extender altura del World Frame"
    L.CVAR_ENABLE_STANCE_PATCH = "Activar parche de cambio de postura/forma"
    L.CVAR_ENABLE_SHOW_PLAYER = "Habilitar la visualización del modelo del personaje del jugador"
    L.CVAR_LABEL_CAMERA_INDIRECT_VISIBILITY = "Visibilidad indirecta de la cámara"
    L.CVAR_LABEL_CAMERA_INDIRECT_ALPHA = "Alpha indirecto de la cámara"
    L.CVAR_LABEL_CAMERA_INDIRECT_OFFSET = "Desplazamiento indirecto de la cámara"
    L.CVAR_LABEL_CAMERA_DISTANCE_MAX = "Distancia de cámara"

    L.DESC_HITBOX_DISABLED = "0 = DESACTIVADO, no usar sin un addon de placas de nombre"
    L.DESC_EXTEND_WORLD_FRAME_HEIGHT = "Fija las placas de nombre fuera de vista en el borde superior; puede romper algunos elementos de la interfaz. REQUIERE apilar placas de nombre activado."
    L.DESC_STANCE_PATCH = "Permite cambiar de postura/forma y lanzar habilidades con macro en un solo clic. No funciona en Warmane."
    L.DESC_CAMERA_INDIRECT_VISIBILITY = "Permite que la cámara se mueva libremente a través de algunos objetos del mundo sin ser bloqueada."
    L.DESC_CAMERA_INDIRECT_ALPHA = "Define la transparencia de los objetos entre la cámara y el jugador."
    L.DESC_CAMERA_DISTANCE_MAX = "Establece la distancia máxima hasta la que puedes alejar la cámara."

    -- CVar Mode Options
    L.MODE_LABEL_REACTION_API = "API de reacción"
    L.MODE_LABEL_COLOR_PARSING = "Análisis de color"
    L.MODE_LABEL_PLAYER_RADIUS = "Radio del jugador 20yd"
    L.MODE_LABEL_CONE_ANGLE = "Ángulo de cono (grados) dentro de 20yd"
end
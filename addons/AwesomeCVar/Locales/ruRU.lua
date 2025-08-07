-- File: ruRU.lua
-- Language: Russian
local addonName, AwesomeCVar = ...

if not AwesomeCVar.L then
    AwesomeCVar.L = {}
end

local L = AwesomeCVar.L

if GetLocale() == "ruRU" then
    -- General
    L.ADDON_NAME = "AwesomeCVar"
    L.ADDON_NAME_SHORT = "Awesome CVar"
    L.MAIN_FRAME_TITLE = "Менеджер Awesome CVar"
    L.RESET_TO = "Сбросить на %s"

    -- Popups
    L.RELOAD_POPUP_TITLE = "Требуется перезагрузка интерфейса"
    L.RELOAD_POPUP_TEXT = "Одно или несколько внесенных вами изменений требуют перезагрузки интерфейса (/reload) для вступления в силу."
    L.RESET_POPUP_TITLE = "Подтвердить сброс по умолчанию"
    L.RESET_POPUP_TEXT = "Вы уверены, что хотите сбросить все значения на значения по умолчанию?"

    -- Chat Messages
    L.MSG_LOADED = "Awesome CVar загружен! Введите /awesome, чтобы открыть менеджер."
    L.MSG_FRAME_RESET = "Положение окна сброшено в центр."
    L.MSG_SET_VALUE = "Установлено %s на %s."
    L.MSG_FRAME_CREATE_ERROR = "Не удалось создать окно AwesomeCVar!"
    L.MSG_UNKNOWN_COMMAND = "Неизвестная команда. Введите /awesome help для списка доступных команд."
    L.MSG_HELP_HEADER = "Команды Awesome CVar:"
    L.MSG_HELP_TOGGLE = "/awesome - Переключить менеджер CVar"
    L.MSG_HELP_SHOW = "/awesome show - Показать менеджер CVar"
    L.MSG_HELP_HIDE = "/awesome hide - Скрыть менеджер CVar"
    L.MSG_HELP_RESET = "/awesome reset - Сбросить положение окна в центр"
    L.MSG_HELP_HELP = "/awesome help - Показать это справочное сообщение"

    -- CVar Categories
    L.CATEGORY_CAMERA = "Камера"
    L.CATEGORY_NAMEPLATES = "Индикаторы здоровья"
    L.CATEGORY_INTERACTION = "Взаимодействие"

    -- CVar Labels & Descriptions
    L.CVAR_LABEL_CAMERA_FOV = "Поле зрения камеры (FoV)"
    L.CVAR_LABEL_ENABLE_STACKING = "Включить наслоение индикаторов"
    L.CVAR_LABEL_STACK_FRIENDLY = "Наслаивать союзные индикаторы |cffff0000(нужен /reload)|r"
    L.CVAR_LABEL_FRIENDLY_DETECT_MODE = "Режим определения союзников |cffff0000(нужен /reload)|r"
    L.CVAR_LABEL_STACKING_FUNCTION = "Функция наслоения |cffff0000(нужен /reload)|r"
    L.CVAR_LABEL_NAMEPLATE_DISTANCE = "Дистанция индикаторов"
    L.CVAR_LABEL_MAX_RAISE_DISTANCE = "Макс. дистанция подъема"
    L.CVAR_LABEL_X_SPACE = "Пробел X для индикаторов"
    L.CVAR_LABEL_Y_SPACE = "Пробел Y для индикаторов"
    L.CVAR_LABEL_UPPER_BORDER = "Смещение верхней границы"
    L.CVAR_LABEL_UPPER_BORDER_ONLY_BOSS = "Только боссы могут прилипать к верхней границе экрана"
    L.CVAR_LABEL_ORIGIN_POS = "Смещение начальной позиции"
    L.CVAR_LABEL_SPEED_RAISE = "Скорость подъема"
    L.CVAR_LABEL_SPEED_RESET = "Скорость сброса"
    L.CVAR_LABEL_SPEED_LOWER = "Скорость опускания"
    L.CVAR_LABEL_HITBOX_HEIGHT = "Высота хитбокса"
    L.CVAR_LABEL_HITBOX_WIDTH = "Ширина хитбокса"
    L.CVAR_LABEL_FRIENDLY_HITBOX_HEIGHT = "Высота хитбокса СОЮЗНИКОВ"
    L.CVAR_LABEL_FRIENDLY_HITBOX_WIDTH = "Ширина хитбокса СОЮЗНИКОВ"
    L.CVAR_LABEL_INTERACTION_MODE = "Режим взаимодействия"
    L.CVAR_LABEL_INTERACTION_ANGLE = "Угол конуса взаимодействия"
    L.CVAR_LABEL_EXTEND_WORLD_FRAME_HEIGHT = "Увеличить высоту World Frame"

    L.DESC_HITBOX_DISABLED = "0 = ОТКЛЮЧЕНО, не используйте без аддона для индикаторов здоровья"
    L.DESC_SMOOTH_FUNCTION_ONLY = "(работает только с Плавноq функцией)"
    L.DESC_EXTEND_WORLD_FRAME_HEIGHT = "Позволяет всегда видеть индикаторы боссов, даже если босс большой, увеличивая высоту World Frame в 5 раз. Может нарушить работу некоторых элементов интерфейса."

    -- CVar Mode Options
    L.MODE_LABEL_REACTION_API = "API реакции"
    L.MODE_LABEL_COLOR_PARSING = "Анализ цвета"
    L.MODE_LABEL_LEGACY = "Наследие (стиль Weakaura)"
    L.MODE_LABEL_SMOOTH = "Плавная функция"
    L.MODE_LABEL_PLAYER_RADIUS = "Радиус игрока 20м"
    L.MODE_LABEL_CONE_ANGLE = "Угол конуса (в градусах) в пределах 20м"
end
-- File: zhTW.lua
-- Language: Traditional Chinese
local addonName, AwesomeCVar = ...

if not AwesomeCVar.L then
    AwesomeCVar.L = {}
end

local L = AwesomeCVar.L

if GetLocale() == "zhTW" then
    -- General
    L.ADDON_NAME = "AwesomeCVar"
    L.ADDON_NAME_SHORT = "Awesome CVar"
    L.MAIN_FRAME_TITLE = "Awesome CVar 管理器"
    L.RESET_TO = "重置為 %s"

    -- Popups
    L.RELOAD_POPUP_TITLE = "需要重載使用者介面"
    L.RELOAD_POPUP_TEXT = "您所做的一項或多項變更需要重載使用者介面（/reload）才能生效。"
    L.RESET_POPUP_TITLE = "確認重置為預設值"
    L.RESET_POPUP_TEXT = "您確定要將所有值恢復為預設值嗎？"

    -- Chat Messages
    L.MSG_LOADED = "Awesome CVar 已載入！輸入 /awesome 開啟管理器。"
    L.MSG_FRAME_RESET = "框架位置已重置到中央。"
    L.MSG_SET_VALUE = "已將 %s 設為 %s。"
    L.MSG_FRAME_CREATE_ERROR = "無法建立 AwesomeCVar 框架！"
    L.MSG_UNKNOWN_COMMAND = "未知指令。輸入 /awesome help 查看可用指令。"
    L.MSG_HELP_HEADER = "Awesome CVar 指令："
    L.MSG_HELP_TOGGLE = "/awesome - 切換 CVar 管理器"
    L.MSG_HELP_SHOW = "/awesome show - 顯示 CVar 管理器"
    L.MSG_HELP_HIDE = "/awesome hide - 隱藏 CVar 管理器"
    L.MSG_HELP_RESET = "/awesome reset - 將框架位置重置到中央"
    L.MSG_HELP_HELP = "/awesome help - 顯示此幫助訊息"

    -- CVar Categories
    L.CATEGORY_CAMERA = "鏡頭"
    L.CATEGORY_NAMEPLATES = "姓名板"
    L.CATEGORY_TEXT_TO_SPEECH = "Text to Speech"
    L.CATEGORY_INTERACTION = "互動"
    L.CATEGORY_OTHER = "其他"

    -- CVar Labels & Descriptions
    L.CVAR_LABEL_TTS_VOICE = "TTS Voice"
    L.CVAR_LABEL_TTS_VOLUME = "TTS Volume"
    L.CVAR_LABEL_TTS_SPEED = "TTS Rate"
    L.CVAR_LABEL_CAMERA_FOV = "鏡頭視野（FoV）"
    L.CVAR_LABEL_ENABLE_STACKING = "啟用姓名板堆疊"
    L.CVAR_LABEL_STACK_FRIENDLY = "堆疊友方姓名板 |cffff0000（需重載）|r"
    L.CVAR_LABEL_FRIENDLY_DETECT_MODE = "友方偵測模式 |cffff0000（需重載）|r"
    L.CVAR_LABEL_NAMEPLATE_DISTANCE = "姓名板距離"
    L.CVAR_LABEL_MAX_RAISE_DISTANCE = "最大抬升距離"
    L.CVAR_LABEL_X_SPACE = "姓名板 X 軸間距"
    L.CVAR_LABEL_Y_SPACE = "姓名板 Y 軸間距"
    L.CVAR_LABEL_UPPER_BORDER = "姓名板上邊界偏移"
    L.CVAR_LABEL_UPPER_BORDER_ONLY_BOSS = "僅允許首領貼齊螢幕上邊界"
    L.CVAR_LABEL_ORIGIN_POS = "姓名板原點偏移"
    L.CVAR_LABEL_SPEED_RAISE = "姓名板抬升速度"
    L.CVAR_LABEL_SPEED_RESET = "姓名板重置速度"
    L.CVAR_LABEL_SPEED_LOWER = "姓名板下降速度"
    L.CVAR_LABEL_HITBOX_HEIGHT = "姓名板點擊框高度"
    L.CVAR_LABEL_HITBOX_WIDTH = "姓名板點擊框寬度"
    L.CVAR_LABEL_FRIENDLY_HITBOX_HEIGHT = "友方姓名板點擊框高度"
    L.CVAR_LABEL_FRIENDLY_HITBOX_WIDTH = "友方姓名板點擊框寬度"
    L.CVAR_LABEL_INTERACTION_MODE = "互動模式"
    L.CVAR_LABEL_INTERACTION_ANGLE = "互動錐形角度"
    L.CVAR_LABEL_EXTEND_WORLD_FRAME_HEIGHT = "延展世界框架高度"
    L.CVAR_ENABLE_STANCE_PATCH = "啟用姿態/形態切換修補"
    L.CVAR_ENABLE_SHOW_PLAYER = "啟用玩家角色模型渲染"
    L.CVAR_LABEL_CAMERA_INDIRECT_VISIBILITY = "鏡頭間接可見性"
    L.CVAR_LABEL_CAMERA_INDIRECT_ALPHA = "鏡頭間接透明度"
    L.CVAR_LABEL_CAMERA_INDIRECT_OFFSET = "鏡頭間接偏移"
    L.CVAR_LABEL_CAMERA_DISTANCE_MAX = "攝影機距離"

    L.DESC_HITBOX_DISABLED = "0 = 停用，若無姓名板插件請勿使用"
    L.DESC_EXTEND_WORLD_FRAME_HEIGHT = "將視野外的姓名板固定到螢幕上邊界；可能會影響部分介面元素。需要啟用姓名板堆疊。"
    L.DESC_STANCE_PATCH = "允許你用巨集單擊切換姿態/形態並施放技能。在 Warmane 伺服器無效。"
    L.DESC_CAMERA_INDIRECT_VISIBILITY = "允許鏡頭自由穿越部分世界物件而不被阻擋。"
    L.DESC_CAMERA_INDIRECT_ALPHA = "設定鏡頭與玩家之間物件的透明度。"
    L.DESC_CAMERA_DISTANCE_MAX = "設定你可以拉遠鏡頭的最遠距離。"

    -- CVar Mode Options
    L.MODE_LABEL_REACTION_API = "反應 API"
    L.MODE_LABEL_COLOR_PARSING = "顏色解析"
    L.MODE_LABEL_PLAYER_RADIUS = "玩家半徑 20 碼"
    L.MODE_LABEL_CONE_ANGLE = "20 碼內錐形角度（度）"
end
-- File: zhCN.lua
-- Language: Simplified Chinese
local addonName, AwesomeCVar = ...

if not AwesomeCVar.L then
    AwesomeCVar.L = {}
end

local L = AwesomeCVar.L

if GetLocale() == "zhCN" then
    -- General
    L.ADDON_NAME = "AwesomeCVar"
    L.ADDON_NAME_SHORT = "Awesome CVar"
    L.MAIN_FRAME_TITLE = "Awesome CVar 管理器"
    L.RESET_TO = "重置为 %s"

    -- Popups
    L.RELOAD_POPUP_TITLE = "需要重载用户界面"
    L.RELOAD_POPUP_TEXT = "您所做的一项或多项更改需要重载用户界面（/reload）才能生效。"
    L.RESET_POPUP_TITLE = "确认重置为默认值"
    L.RESET_POPUP_TEXT = "您确定要将所有值恢复为默认值吗？"

    -- Chat Messages
    L.MSG_LOADED = "Awesome CVar 已加载！输入 /awesome 打开管理器。"
    L.MSG_FRAME_RESET = "框架位置已重置到中央。"
    L.MSG_SET_VALUE = "已将 %s 设为 %s。"
    L.MSG_FRAME_CREATE_ERROR = "无法创建 AwesomeCVar 框架！"
    L.MSG_UNKNOWN_COMMAND = "未知命令。输入 /awesome help 查看可用命令。"
    L.MSG_HELP_HEADER = "Awesome CVar 命令："
    L.MSG_HELP_TOGGLE = "/awesome - 切换 CVar 管理器"
    L.MSG_HELP_SHOW = "/awesome show - 显示 CVar 管理器"
    L.MSG_HELP_HIDE = "/awesome hide - 隐藏 CVar 管理器"
    L.MSG_HELP_RESET = "/awesome reset - 将框架位置重置到中央"
    L.MSG_HELP_HELP = "/awesome help - 显示此帮助信息"

    -- CVar Categories
    L.CATEGORY_CAMERA = "镜头"
    L.CATEGORY_NAMEPLATES = "姓名板"
    L.CATEGORY_TEXT_TO_SPEECH = "Text to Speech"
    L.CATEGORY_INTERACTION = "互动"
    L.CATEGORY_OTHER = "其他"

    -- CVar Labels & Descriptions
    L.CVAR_LABEL_TTS_VOICE = "TTS Voice"
    L.CVAR_LABEL_TTS_VOLUME = "TTS Volume"
    L.CVAR_LABEL_TTS_SPEED = "TTS Rate"
    L.CVAR_LABEL_CAMERA_FOV = "镜头视野（FoV）"
    L.CVAR_LABEL_ENABLE_STACKING = "启用姓名板堆叠"
    L.CVAR_LABEL_STACK_FRIENDLY = "堆叠友方姓名板 |cffff0000（需重载）|r"
    L.CVAR_LABEL_FRIENDLY_DETECT_MODE = "友方侦测模式 |cffff0000（需重载）|r"
    L.CVAR_LABEL_NAMEPLATE_DISTANCE = "姓名板距离"
    L.CVAR_LABEL_MAX_RAISE_DISTANCE = "最大抬升距离"
    L.CVAR_LABEL_X_SPACE = "姓名板 X 轴间距"
    L.CVAR_LABEL_Y_SPACE = "姓名板 Y 轴间距"
    L.CVAR_LABEL_UPPER_BORDER = "姓名板上边界偏移"
    L.CVAR_LABEL_UPPER_BORDER_ONLY_BOSS = "仅允许首领贴合屏幕上边界"
    L.CVAR_LABEL_ORIGIN_POS = "姓名板原点偏移"
    L.CVAR_LABEL_SPEED_RAISE = "姓名板抬升速度"
    L.CVAR_LABEL_SPEED_RESET = "姓名板重置速度"
    L.CVAR_LABEL_SPEED_LOWER = "姓名板下降速度"
    L.CVAR_LABEL_HITBOX_HEIGHT = "姓名板点击框高度"
    L.CVAR_LABEL_HITBOX_WIDTH = "姓名板点击框宽度"
    L.CVAR_LABEL_FRIENDLY_HITBOX_HEIGHT = "友方姓名板点击框高度"
    L.CVAR_LABEL_FRIENDLY_HITBOX_WIDTH = "友方姓名板点击框宽度"
    L.CVAR_LABEL_INTERACTION_MODE = "互动模式"
    L.CVAR_LABEL_INTERACTION_ANGLE = "互动锥形角度"
    L.CVAR_LABEL_EXTEND_WORLD_FRAME_HEIGHT = "扩展世界框架高度"
    L.CVAR_ENABLE_STANCE_PATCH = "启用姿态/形态切换补丁"
    L.CVAR_ENABLE_SHOW_PLAYER = "启用玩家角色模型渲染"
    L.CVAR_LABEL_CAMERA_INDIRECT_VISIBILITY = "镜头间接可见性"
    L.CVAR_LABEL_CAMERA_INDIRECT_ALPHA = "镜头间接透明度"
    L.CVAR_LABEL_CAMERA_INDIRECT_OFFSET = "镜头间接偏移"
    L.CVAR_LABEL_CAMERA_DISTANCE_MAX = "摄像机距离"

    L.DESC_HITBOX_DISABLED = "0 = 禁用，若无姓名板插件请勿使用"
    L.DESC_EXTEND_WORLD_FRAME_HEIGHT = "将视野外的姓名板固定到屏幕上边界；可能会影响部分界面元素。需要启用姓名板堆叠。"
    L.DESC_STANCE_PATCH = "允许你通过宏一键切换姿态/形态并施放技能。在 Warmane 服务器无效。"
    L.DESC_CAMERA_INDIRECT_VISIBILITY = "允许镜头自由穿越部分世界物体而不被阻挡。"
    L.DESC_CAMERA_INDIRECT_ALPHA = "定义镜头与玩家之间物体的透明度。"
    L.DESC_CAMERA_DISTANCE_MAX = "设置你可以拉远镜头的最大距离。"

    -- CVar Mode Options
    L.MODE_LABEL_REACTION_API = "反应 API"
    L.MODE_LABEL_COLOR_PARSING = "颜色解析"
    L.MODE_LABEL_PLAYER_RADIUS = "玩家半径 20 码"
    L.MODE_LABEL_CONE_ANGLE = "20 码内锥形角度（度）"
end
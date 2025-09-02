-- File: koKR.lua
-- Language: Korean
local addonName, AwesomeCVar = ...

if not AwesomeCVar.L then
    AwesomeCVar.L = {}
end

local L = AwesomeCVar.L

if GetLocale() == "koKR" then
    -- General
    L.ADDON_NAME = "AwesomeCVar"
    L.ADDON_NAME_SHORT = "Awesome CVar"
    L.MAIN_FRAME_TITLE = "Awesome CVar 관리자"
    L.RESET_TO = "%s (으)로 초기화"

    -- Popups
    L.RELOAD_POPUP_TITLE = "UI 새로고침 필요"
    L.RELOAD_POPUP_TEXT = "변경하신 내용 중 하나 이상을 적용하려면 UI를 새로고침(/reload)해야 합니다."
    L.RESET_POPUP_TITLE = "기본값 초기화 확인"
    L.RESET_POPUP_TEXT = "모든 값을 기본값으로 되돌리시겠습니까?"

    -- Chat Messages
    L.MSG_LOADED = "Awesome CVar 로드 완료! /awesome 를 입력하여 관리자를 여세요."
    L.MSG_FRAME_RESET = "창 위치가 중앙으로 초기화되었습니다."
    L.MSG_SET_VALUE = "%s 을(를) %s (으)로 설정했습니다."
    L.MSG_FRAME_CREATE_ERROR = "AwesomeCVar 창을 생성할 수 없습니다!"
    L.MSG_UNKNOWN_COMMAND = "알 수 없는 명령어입니다. /awesome help 로 사용 가능한 명령어를 확인하세요."
    L.MSG_HELP_HEADER = "Awesome CVar 명령어:"
    L.MSG_HELP_TOGGLE = "/awesome - CVar 관리자 창을 토글합니다"
    L.MSG_HELP_SHOW = "/awesome show - CVar 관리자 창을 엽니다"
    L.MSG_HELP_HIDE = "/awesome hide - CVar 관리자 창을 닫습니다"
    L.MSG_HELP_RESET = "/awesome reset - 창 위치를 중앙으로 초기화합니다"
    L.MSG_HELP_HELP = "/awesome help - 이 도움말 메시지를 표시합니다"

    -- CVar Categories
    L.CATEGORY_CAMERA = "카메라"
    L.CATEGORY_NAMEPLATES = "이름표"
    L.CATEGORY_TEXT_TO_SPEECH = "Text to Speech"
    L.CATEGORY_INTERACTION = "상호작용"
    L.CATEGORY_OTHER = "기타"

    -- CVar Labels & Descriptions
    L.CVAR_LABEL_TTS_VOICE = "TTS Voice"
    L.CVAR_LABEL_TTS_VOLUME = "TTS Volume"
    L.CVAR_LABEL_TTS_SPEED = "TTS Rate"
    L.CVAR_LABEL_CAMERA_FOV = "카메라 시야각 (FoV)"
    L.CVAR_LABEL_ENABLE_STACKING = "이름표 겹치기 활성화"
    L.CVAR_LABEL_STACK_FRIENDLY = "우호적 이름표 겹치기 |cffff0000(새로고침 필요)|r"
    L.CVAR_LABEL_FRIENDLY_DETECT_MODE = "우호적 대상 감지 모드 |cffff0000(새로고침 필요)|r"
    L.CVAR_LABEL_NAMEPLATE_DISTANCE = "이름표 거리"
    L.CVAR_LABEL_MAX_RAISE_DISTANCE = "최대 상승 거리"
    L.CVAR_LABEL_X_SPACE = "이름표 X축 간격"
    L.CVAR_LABEL_Y_SPACE = "이름표 Y축 간격"
    L.CVAR_LABEL_UPPER_BORDER = "이름표 상단 테두리 오프셋"
    L.CVAR_LABEL_UPPER_BORDER_ONLY_BOSS = "보스만 화면 상단 테두리에 고정 허용"
    L.CVAR_LABEL_ORIGIN_POS = "이름표 기준점 오프셋"
    L.CVAR_LABEL_SPEED_RAISE = "이름표 상승 속도"
    L.CVAR_LABEL_SPEED_RESET = "이름표 초기화 속도"
    L.CVAR_LABEL_SPEED_LOWER = "이름표 하강 속도"
    L.CVAR_LABEL_HITBOX_HEIGHT = "이름표 히트박스 높이"
    L.CVAR_LABEL_HITBOX_WIDTH = "이름표 히트박스 너비"
    L.CVAR_LABEL_FRIENDLY_HITBOX_HEIGHT = "우호적 이름표 히트박스 높이"
    L.CVAR_LABEL_FRIENDLY_HITBOX_WIDTH = "우호적 이름표 히트박스 너비"
    L.CVAR_LABEL_INTERACTION_MODE = "상호작용 모드"
    L.CVAR_LABEL_INTERACTION_ANGLE = "상호작용 원뿔 각도"
    L.CVAR_LABEL_EXTEND_WORLD_FRAME_HEIGHT = "월드 프레임 높이 확장"
    L.CVAR_ENABLE_STANCE_PATCH = "태세/폼 전환 패치 활성화"
    L.CVAR_ENABLE_SHOW_PLAYER = "플레이어 캐릭터 모델 렌더링 활성화"
    L.CVAR_LABEL_CAMERA_INDIRECT_VISIBILITY = "카메라 간접 가시성"
    L.CVAR_LABEL_CAMERA_INDIRECT_ALPHA = "카메라 간접 투명도"
    L.CVAR_LABEL_CAMERA_INDIRECT_OFFSET = "카메라 간접 오프셋"
    L.CVAR_LABEL_CAMERA_DISTANCE_MAX = "카메라 거리"

    L.DESC_HITBOX_DISABLED = "0 = 비활성화됨, 이름표 애드온 없이 사용하지 마세요"
    L.DESC_EXTEND_WORLD_FRAME_HEIGHT = "화면 밖 이름표를 상단 테두리에 고정합니다. 일부 UI 요소가 깨질 수 있습니다. 이름표 겹치기 활성화 필요."
    L.DESC_STANCE_PATCH = "매크로로 한 번 클릭 시 태세/폼 변경과 기술 시전이 가능합니다. Warmane 서버에서는 작동하지 않습니다."
    L.DESC_CAMERA_INDIRECT_VISIBILITY = "카메라가 일부 월드 오브젝트를 자유롭게 통과할 수 있도록 허용합니다."
    L.DESC_CAMERA_INDIRECT_ALPHA = "카메라와 플레이어 사이 오브젝트의 투명도를 정의합니다."
    L.DESC_CAMERA_DISTANCE_MAX = "카메라를 얼마나 멀리 줌아웃할 수 있는지의 최대 거리를 설정합니다."

    -- CVar Mode Options
    L.MODE_LABEL_REACTION_API = "반응 API"
    L.MODE_LABEL_COLOR_PARSING = "색상 분석"
    L.MODE_LABEL_PLAYER_RADIUS = "플레이어 반경 20미터"
    L.MODE_LABEL_CONE_ANGLE = "20미터 내 원뿔 각도 (도)"
end
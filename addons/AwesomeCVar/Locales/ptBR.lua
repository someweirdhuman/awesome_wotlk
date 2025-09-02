-- File: ptBR.lua
-- Language: Portuguese (Brazil)
local addonName, AwesomeCVar = ...

if not AwesomeCVar.L then
    AwesomeCVar.L = {}
end

local L = AwesomeCVar.L

if GetLocale() == "ptBR" then
    -- General
    L.ADDON_NAME = "AwesomeCVar"
    L.ADDON_NAME_SHORT = "Awesome CVar"
    L.MAIN_FRAME_TITLE = "Gerenciador Awesome CVar"
    L.RESET_TO = "Redefinir para %s"

    -- Popups
    L.RELOAD_POPUP_TITLE = "É Necessário Recarregar a Interface"
    L.RELOAD_POPUP_TEXT = "Uma ou mais das alterações que você fez requerem que a interface seja recarregada (/reload) para terem efeito."
    L.RESET_POPUP_TITLE = "Confirmar Redefinição para Padrões"
    L.RESET_POPUP_TEXT = "Você tem certeza de que deseja redefinir todos os valores para os padrões?"

    -- Chat Messages
    L.MSG_LOADED = "Awesome CVar carregado! Digite /awesome para abrir o gerenciador."
    L.MSG_FRAME_RESET = "A posição da janela foi redefinida para o centro."
    L.MSG_SET_VALUE = "Definido %s para %s."
    L.MSG_FRAME_CREATE_ERROR = "A janela do AwesomeCVar não pôde ser criada!"
    L.MSG_UNKNOWN_COMMAND = "Comando desconhecido. Digite /awesome help para os comandos disponíveis."
    L.MSG_HELP_HEADER = "Comandos do Awesome CVar:"
    L.MSG_HELP_TOGGLE = "/awesome - Alterna o gerenciador de CVar"
    L.MSG_HELP_SHOW = "/awesome show - Mostra o gerenciador de CVar"
    L.MSG_HELP_HIDE = "/awesome hide - Oculta o gerenciador de CVar"
    L.MSG_HELP_RESET = "/awesome reset - Redefine a posição da janela para o centro"
    L.MSG_HELP_HELP = "/awesome help - Mostra esta mensagem de ajuda"

    -- CVar Categories
    L.CATEGORY_CAMERA = "Câmera"
    L.CATEGORY_NAMEPLATES = "Placas de Nome"
    L.CATEGORY_TEXT_TO_SPEECH = "Text to Speech"
    L.CATEGORY_INTERACTION = "Interação"
    L.CATEGORY_OTHER = "Outros"

    -- CVar Labels & Descriptions
    L.CVAR_LABEL_TTS_VOICE = "TTS Voice"
    L.CVAR_LABEL_TTS_VOLUME = "TTS Volume"
    L.CVAR_LABEL_TTS_SPEED = "TTS Rate"
    L.CVAR_LABEL_CAMERA_FOV = "Campo de Visão (FoV) da Câmera"
    L.CVAR_LABEL_ENABLE_STACKING = "Ativar Empilhamento de Placas de Nome"
    L.CVAR_LABEL_STACK_FRIENDLY = "Empilhar Placas de Nome Aliadas |cffff0000(Req. Recarregar)|r"
    L.CVAR_LABEL_FRIENDLY_DETECT_MODE = "Modo de Detecção de Aliados |cffff0000(Req. Recarregar)|r"
    L.CVAR_LABEL_NAMEPLATE_DISTANCE = "Distância das Placas de Nome"
    L.CVAR_LABEL_MAX_RAISE_DISTANCE = "Distância Máxima de Elevação"
    L.CVAR_LABEL_X_SPACE = "Espaço X das Placas de Nome"
    L.CVAR_LABEL_Y_SPACE = "Espaço Y das Placas de Nome"
    L.CVAR_LABEL_UPPER_BORDER = "Deslocamento da Borda Superior"
    L.CVAR_LABEL_UPPER_BORDER_ONLY_BOSS = "Permitir SOMENTE chefes grudarem na borda superior da tela"
    L.CVAR_LABEL_ORIGIN_POS = "Deslocamento de Origem"
    L.CVAR_LABEL_SPEED_RAISE = "Velocidade de Elevação"
    L.CVAR_LABEL_SPEED_RESET = "Velocidade de Redefinição"
    L.CVAR_LABEL_SPEED_LOWER = "Velocidade de Descida"
    L.CVAR_LABEL_HITBOX_HEIGHT = "Altura da Hitbox"
    L.CVAR_LABEL_HITBOX_WIDTH = "Largura da Hitbox"
    L.CVAR_LABEL_FRIENDLY_HITBOX_HEIGHT = "Altura da Hitbox ALIADA"
    L.CVAR_LABEL_FRIENDLY_HITBOX_WIDTH = "Largura da Hitbox ALIADA"
    L.CVAR_LABEL_INTERACTION_MODE = "Modo de Interação"
    L.CVAR_LABEL_INTERACTION_ANGLE = "Ângulo do Cone de Interação"
    L.CVAR_LABEL_EXTEND_WORLD_FRAME_HEIGHT = "Estender Altura do World Frame"
    L.CVAR_ENABLE_STANCE_PATCH = "Ativar patch de troca de postura/forma"
    L.CVAR_ENABLE_SHOW_PLAYER = "Habilitar renderização do modelo do personagem do jogador"
    L.CVAR_LABEL_CAMERA_INDIRECT_VISIBILITY = "Visibilidade Indireta da Câmera"
    L.CVAR_LABEL_CAMERA_INDIRECT_ALPHA = "Alpha Indireto da Câmera"
    L.CVAR_LABEL_CAMERA_INDIRECT_OFFSET = "Offset Indireto da Câmera"
    L.CVAR_LABEL_CAMERA_DISTANCE_MAX = "Distância da câmera"

    L.DESC_HITBOX_DISABLED = "0 = DESATIVADO, não use sem um addon de placas de nome"
    L.DESC_EXTEND_WORLD_FRAME_HEIGHT = "Gruda placas de nome fora de visão na borda superior; pode quebrar alguns elementos da interface. REQUER empilhamento de placas de nome ativado."
    L.DESC_STANCE_PATCH = "Permite trocar postura/forma e lançar habilidade via macro com um único clique. Não funciona no Warmane."
    L.DESC_CAMERA_INDIRECT_VISIBILITY = "Permite que sua câmera se mova livremente por alguns objetos do mundo sem ser bloqueada."
    L.DESC_CAMERA_INDIRECT_ALPHA = "Define a transparência dos objetos entre a câmera e o jogador."
    L.DESC_CAMERA_DISTANCE_MAX = "Define a distância máxima para a qual você pode afastar a câmera."

    -- CVar Mode Options
    L.MODE_LABEL_REACTION_API = "API de Reação"
    L.MODE_LABEL_COLOR_PARSING = "Análise de Cor"
    L.MODE_LABEL_PLAYER_RADIUS = "Raio do Jogador 20m"
    L.MODE_LABEL_CONE_ANGLE = "Ângulo do cone (graus) dentro de 20m"
end
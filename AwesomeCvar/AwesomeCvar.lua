local AwesomeCvar = {}

local CVARS_DEFINITIONS = {
    {
        name = "cameraFov",
        label = "Camera Fov",
        type = "slider",
        min = 30,
        max = 150,
    },
    {
        name = "nameplateStacking",
        label = "Enable Nameplate Stacking",
        type = "toggle",
        min = 0,
        max = 1,
    },
    {
        name = "nameplateStackFriendly",
        label = "Stack Friendly Nameplates (/reload)",
        type = "toggle",
        min = 0,
        max = 1,
    },
    {
        name = "nameplateStackFriendlyMode",
        label = "Friendly Detection Mode (/reload)",
        type = "mode",
        modes = {
            {value = 0, label = "Reaction API"},
            {value = 1, label = "Color Parsing"},
        },
    },
    {
        name = "nameplateStackFunction",
        label = "Stacking Function (/reload)",
        type = "mode",
        modes = {
            {value = 0, label = "Legacy (Weakaura-style)"},
            {value = 1, label = "Smooth Function"},
        },
    },
    {
        name = "nameplateDistance",
        label = "Nameplate Distance",
        type = "slider",
        min = 1,
        max = 200,
        step = 1
    },
    {
        name = "nameplateMaxRaiseDistance",
        label = "Max Raise Distance (only works with Smooth Function)",
        type = "slider",
        min = 0,
        max = 500,
        step = 1
    },
    {
        name = "nameplateXSpace",
        label = "Nameplate X Space",
        type = "slider",
        min = 5,
        max = 200,
        step = 1
    },
    {
        name = "nameplateYSpace",
        label = "Nameplate Y Space",
        type = "slider",
        min = 5,
        max = 75,
        step = 1
    },
    {
        name = "nameplateUpperBorder",
        label = "Nameplate Upper Border Offset",
        type = "slider",
        min = 0,
        max = 100,
        step = 1
    },
    {
        name = "nameplateOriginPos",
        label = "Nameplate Origin Offset",
        type = "slider",
        min = 0,
        max = 60,
        step = 1
    },
    {
        name = "nameplateSpeedRaise",
        label = "Nameplate Speed Raise",
        type = "slider",
        min = 0,
        max = 5,
        step = 0.1
    },
    {
        name = "nameplateSpeedReset",
        label = "Nameplate Speed Reset",
        type = "slider",
        min = 0,
        max = 5,
        step = 0.1
    },
    {
        name = "nameplateSpeedLower",
        label = "Nameplate Speed Lower",
        type = "slider",
        min = 0,
        max = 5,
        step = 0.1
    },
    {
        name = "nameplateHitboxHeight",
        label = "Nameplate Hitbox Height (CHANGES SIZE OF DEFAULT PLATES)",
        type = "slider",
        min = 0,
        max = 50,
        step = 1
    },
    {
        name = "nameplateHitboxWidth",
        label = "Nameplate Hitbox Width (CHANGES SIZE OF DEFAULT PLATES)",
        type = "slider",
        min = 0,
        max = 200,
        step = 1
    },
    {
        name = "nameplateFriendlyHitboxHeight",
        label = "FRIENDLY Nameplate Hitbox Height (CHANGES SIZE OF DEFAULT PLATES)",
        type = "slider",
        min = 0,
        max = 50,
        step = 1
    },
    {
        name = "nameplateFriendlyHitboxWidth",
        label = "FRIENDLY Nameplate Hitbox Width (CHANGES SIZE OF DEFAULT PLATES)",
        type = "slider",
        min = 0,
        max = 200,
        step = 1
    },
    {
        name = "interactionMode",
        label = "Interaction Mode",
        type = "mode",
        modes = {
            {value = 0, label = "Player Radius 20yd"},
            {value = 1, label = "Cone Angle (dg) within 20yd"},
        },
    },
    {
        name = "interactionAngle",
        label = "Interaction Cone Angle",
        type = "slider",
        min = 1,
        max = 360,
        step = 1
    },
}

function AwesomeCvar.GetCVarValue(cvarName)
    local value = GetCVar(cvarName)
    return tonumber(value) or value
end

function AwesomeCvar.SetCVarValue(cvarName, value)
    SetCVar(cvarName, value)
end

function AwesomeCvar.PrintValue(cvarName, value)
    DEFAULT_CHAT_FRAME:AddMessage("|cff00ff00Awesome Cvar:|r Set |cffffd100"..cvarName.."|r to |cff00ccff"..tostring(value).."|r.")
end

function AwesomeCvar.UpdateUIForCVar(cvarDef)
    local cvarName = cvarDef.name
    local currentValue = AwesomeCvar.GetCVarValue(cvarName)

    if cvarDef.type == "toggle" then
        local checkbox = _G["AwesomeCvar_" .. cvarName .. "Checkbox"]
        if checkbox then checkbox:SetChecked(currentValue == cvarDef.max) end
    elseif cvarDef.type == "slider" then
        local slider = _G["AwesomeCvar_" .. cvarName .. "Slider"]
        local valueText = _G["AwesomeCvar_" .. cvarName .. "SliderValue"]
        if slider then
            slider:SetValue(currentValue)
            if valueText then valueText:SetText(tostring(currentValue)) end
        end
    elseif cvarDef.type == "mode" then
        for i, modeDef in ipairs(cvarDef.modes) do
            local radioButton = _G["AwesomeCvar_" .. cvarName .. "Radio" .. i]
            if radioButton then
                radioButton:SetChecked(currentValue == modeDef.value)
            end
        end
    end
end

function AwesomeCvar.UpdateAllUI()
    for _, cvarDef in ipairs(CVARS_DEFINITIONS) do
        AwesomeCvar.UpdateUIForCVar(cvarDef)
    end
end

function AwesomeCvar.CreateMainFrame()
    local frame = CreateFrame("Frame", "AwesomeCvarFrame", UIParent, "AwesomeCvarFrameTemplate")
    frame:SetPoint("CENTER")
    frame:Hide()

    local title = frame:CreateFontString(nil, "OVERLAY", "GameFontNormalLarge")
    title:SetPoint("TOP", frame, "TOP", 0, -10)
    title:SetText("Awesome Cvar Manager")

    local close = CreateFrame("Button", nil, frame, "UIPanelCloseButton")
    close:SetPoint("TOPRIGHT", -5, -5)
    close:SetScript("OnClick", function() frame:Hide() end)

    local scrollFrame = CreateFrame("ScrollFrame", "AwesomeCvarScrollFrame", frame, "UIPanelScrollFrameTemplate")
    scrollFrame:SetPoint("TOPLEFT", frame, "TOPLEFT", 16, -40)
    scrollFrame:SetPoint("BOTTOMRIGHT", frame, "BOTTOMRIGHT", -30, 10)

    local contentFrame = CreateFrame("Frame", "AwesomeCvarContentFrame", scrollFrame)
    contentFrame:SetWidth(340)
    contentFrame:SetHeight(400)
    scrollFrame:SetScrollChild(contentFrame)

    local lastY = -10

    for _, cvarDef in ipairs(CVARS_DEFINITIONS) do
        local cvarName = cvarDef.name
        local label = cvarDef.label

        local control = CreateFrame("Frame", "AwesomeCvar_" .. cvarName .. "Control", contentFrame)
        control:SetPoint("TOPLEFT", contentFrame, "TOPLEFT", 10, lastY)
        control:SetWidth(320)

        local text = control:CreateFontString(nil, "ARTWORK", "GameFontNormal")
        text:SetPoint("TOPLEFT", 0, 0)
        text:SetText(label .. ":")

        if cvarDef.type == "toggle" then
            control:SetHeight(25)
            local checkbox = CreateFrame("CheckButton", "AwesomeCvar_" .. cvarName .. "Checkbox", control, "UICheckButtonTemplate")
            checkbox:SetPoint("LEFT", text, "RIGHT", 10, 0)
            checkbox.cvarDef = cvarDef
            checkbox:SetScript("OnClick", function(self)
                local newVal = self:GetChecked() and self.cvarDef.max or self.cvarDef.min
                AwesomeCvar.SetCVarValue(self.cvarDef.name, newVal)
                AwesomeCvar.PrintValue(self.cvarDef.name, newVal)
            end)
            lastY = lastY - 35
        elseif cvarDef.type == "slider" then
            control:SetHeight(40)
            local slider = CreateFrame("Slider", "AwesomeCvar_" .. cvarName .. "Slider", control, "OptionsSliderTemplate")
            slider:SetMinMaxValues(cvarDef.min, cvarDef.max)
            slider:SetValueStep(cvarDef.step or 1)
            slider:SetPoint("TOPLEFT", text, "BOTTOMLEFT", 0, -10)
            slider:SetPoint("RIGHT", control, "RIGHT", -10, 0)

            local valueText = control:CreateFontString("AwesomeCvar_" .. cvarName .. "SliderValue", "ARTWORK", "GameFontNormal")
            valueText:SetPoint("TOP", slider, "BOTTOM", 0, 0)

            slider.cvarDef = cvarDef
            slider.valueText = valueText
            slider:SetScript("OnValueChanged", function(self, val)
                val = tonumber(string.format("%.2f", val))
                self.valueText:SetText(tostring(val))
                AwesomeCvar.SetCVarValue(self.cvarDef.name, val)
                self.pendingValue = val 
            end)

            slider:SetScript("OnMouseUp", function(self)
                if self.pendingValue then
                    AwesomeCvar.PrintValue(self.cvarDef.name, self.pendingValue)
                end
            end)
            lastY = lastY - 60
        elseif cvarDef.type == "mode" then
            local offsetY = -20
            for i, mode in ipairs(cvarDef.modes) do
                local radio = CreateFrame("CheckButton", "AwesomeCvar_" .. cvarName .. "Radio" .. i, control, "UIRadioButtonTemplate")
                radio:SetPoint("TOPLEFT", text, "BOTTOMLEFT", 0, offsetY)
                local labelText = radio:CreateFontString(nil, "ARTWORK", "GameFontHighlight")
                labelText:SetPoint("LEFT", radio, "RIGHT", 5, 0)
                labelText:SetText(mode.label)
                radio.cvarDef = cvarDef
                radio.modeValue = mode.value
                radio:SetScript("OnClick", function(self)
                    for j = 1, #self.cvarDef.modes do
                        _G["AwesomeCvar_" .. self.cvarDef.name .. "Radio" .. j]:SetChecked(false)
                    end
                    self:SetChecked(true)
                    AwesomeCvar.SetCVarValue(self.cvarDef.name, self.modeValue)
                    AwesomeCvar.PrintValue(self.cvarDef.name, self.modeValue)
                end)
                offsetY = offsetY - 25
            end
            control:SetHeight(math.abs(offsetY) + 10)
            lastY = lastY + offsetY - 10
        end
    end

    contentFrame:SetHeight(math.abs(lastY) + 50)
    AwesomeCvar.Frame = frame
end

function AwesomeCvar.OnLoad()
    AwesomeCvar.CreateMainFrame()
    AwesomeCvar.UpdateAllUI()
end

SLASH_AWESOME1 = "/awesome"
SlashCmdList["AWESOME"] = function()
    if AwesomeCvar.Frame:IsShown() then
        AwesomeCvar.Frame:Hide()
    else
        AwesomeCvar.Frame:Show()
        AwesomeCvar.UpdateAllUI()
    end
end

local initFrame = CreateFrame("Frame")
initFrame:RegisterEvent("ADDON_LOADED")
initFrame:SetScript("OnEvent", function(_, event, arg)
    if arg == "AwesomeCvar" then
        AwesomeCvar.OnLoad()
    end
end)
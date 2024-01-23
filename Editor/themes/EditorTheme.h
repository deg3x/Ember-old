#pragma once
#include "imgui/imgui.h"

class EditorTheme
{
public:
    static constexpr ImVec4 ThemeColorGreen        = ImVec4(0.10f, 0.55f, 0.45f, 1.00f);
    static constexpr ImVec4 ThemeColorGreenHovered = ImVec4(0.10f, 0.55f, 0.45f, 0.75f);
    static constexpr ImVec4 ThemeColorGreenActive  = ImVec4(0.10f, 0.55f, 0.45f, 0.40f);
    
    static constexpr ImVec4 ThemeColorBlue         = ImVec4(0.10f, 0.50f, 0.70f, 1.00f);
    static constexpr ImVec4 ThemeColorBlueHovered  = ImVec4(0.10f, 0.50f, 0.70f, 0.75f);
    static constexpr ImVec4 ThemeColorBlueActive   = ImVec4(0.10f, 0.50f, 0.70f, 0.40f);

    static constexpr ImVec4 ThemeColorTextInfo     = ImVec4(0.90f, 0.90f, 0.90f, 1.00f);
    static constexpr ImVec4 ThemeColorTextWarning  = ImVec4(0.80f, 0.75f, 0.30f, 1.00f);
    static constexpr ImVec4 ThemeColorTextError    = ImVec4(0.90f, 0.30f, 0.10f, 1.00f);
public:
    EditorTheme() = delete;
    
    static void ApplyTheme();
};

#pragma once
#include "imgui/imgui.h"

class EditorTheme
{
public:
    static constexpr ImVec4 ColorGreen        = ImVec4(0.10f, 0.55f, 0.45f, 1.00f);
    static constexpr ImVec4 ColorGreenHovered = ImVec4(0.10f, 0.55f, 0.45f, 0.75f);
    static constexpr ImVec4 ColorGreenActive  = ImVec4(0.10f, 0.55f, 0.45f, 0.40f);
    
    static constexpr ImVec4 ColorBlue         = ImVec4(0.10f, 0.50f, 0.70f, 1.00f);
    static constexpr ImVec4 ColorBlueHovered  = ImVec4(0.10f, 0.50f, 0.70f, 0.75f);
    static constexpr ImVec4 ColorBlueActive   = ImVec4(0.10f, 0.50f, 0.70f, 0.40f);

    static constexpr ImVec4 ColorTextInfo     = ImVec4(0.90f, 0.90f, 0.90f, 1.00f);
    static constexpr ImVec4 ColorTextWarning  = ImVec4(0.80f, 0.75f, 0.30f, 1.00f);
    static constexpr ImVec4 ColorTextError    = ImVec4(0.90f, 0.30f, 0.10f, 1.00f);

    inline static ImFont* FontRegular;
    inline static ImFont* FontMedium;
    inline static ImFont* FontLight;
    inline static ImFont* FontBold;
public:
    EditorTheme() = delete;
    
    static void ApplyTheme();
};

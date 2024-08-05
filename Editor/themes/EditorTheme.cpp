#include "editor_pch.h"
#include "EditorTheme.h"

#include "EditorThemeLoader.h"

#include "utils/PathBuilder.h"

void EditorTheme::ApplyTheme()
{
    const ImGuiIO& io = ImGui::GetIO();
    FontRegular = io.Fonts->AddFontFromFileTTF(PathBuilder::GetPath("./Data/fonts/Roboto/Roboto-Regular.ttf").c_str(), 16);
    FontMedium  = io.Fonts->AddFontFromFileTTF(PathBuilder::GetPath("./Data/fonts/Roboto/Roboto-Medium.ttf").c_str(), 16);
    FontLight   = io.Fonts->AddFontFromFileTTF(PathBuilder::GetPath("./Data/fonts/Roboto/Roboto-Light.ttf").c_str(), 16);
    FontBold    = io.Fonts->AddFontFromFileTTF(PathBuilder::GetPath("./Data/fonts/Roboto/Roboto-Bold.ttf").c_str(), 16);

    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowPadding                     = ImVec2(6.00f, 6.00f);
    style.FramePadding                      = ImVec2(6.00f, 3.00f);
    style.CellPadding                       = ImVec2(6.00f, 6.00f);
    style.ItemSpacing                       = ImVec2(6.00f, 6.00f);
    style.ItemInnerSpacing                  = ImVec2(6.00f, 6.00f);
    style.TouchExtraPadding                 = ImVec2(0.00f, 0.00f);
    style.IndentSpacing                     = 25;
    style.ScrollbarSize                     = 12;
    style.GrabMinSize                       = 10;
    style.WindowBorderSize                  = 1;
    style.ChildBorderSize                   = 1;
    style.PopupBorderSize                   = 1;
    style.FrameBorderSize                   = 0;
    style.TabBorderSize                     = 1;
    style.WindowRounding                    = 7;
    style.ChildRounding                     = 0;
    style.FrameRounding                     = 3;
    style.PopupRounding                     = 4;
    style.ScrollbarRounding                 = 9;
    style.GrabRounding                      = 3;
    style.LogSliderDeadzone                 = 4;
    style.TabRounding                       = 4;
    style.WindowMinSize                     = ImVec2(250.0f, 250.0f);

    
    ImGuizmo::Style& gizmoStyle = ImGuizmo::GetStyle();
    gizmoStyle.Colors[ImGuizmo::COLOR::DIRECTION_X]        = ImVec4(InspectorColorX.x, InspectorColorX.y, InspectorColorX.z, 1.0f);
    gizmoStyle.Colors[ImGuizmo::COLOR::DIRECTION_Y]        = ImVec4(InspectorColorY.x, InspectorColorY.y, InspectorColorY.z, 1.0f);
    gizmoStyle.Colors[ImGuizmo::COLOR::DIRECTION_Z]        = ImVec4(InspectorColorZ.x, InspectorColorZ.y, InspectorColorZ.z, 1.0f);
    gizmoStyle.Colors[ImGuizmo::COLOR::PLANE_X]            = InspectorColorX;
    gizmoStyle.Colors[ImGuizmo::COLOR::PLANE_Y]            = InspectorColorY;
    gizmoStyle.Colors[ImGuizmo::COLOR::PLANE_Z]            = InspectorColorZ;
    gizmoStyle.Colors[ImGuizmo::COLOR::HATCHED_AXIS_LINES] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);

    gizmoStyle.CenterCircleSize = 5.0f;
    
    gizmoStyle.TranslationLineThickness   = 4.0f;
    gizmoStyle.TranslationLineArrowSize   = 6.0f;
    gizmoStyle.RotationLineThickness      = 3.0f;
    gizmoStyle.RotationOuterLineThickness = 2.0f;
    gizmoStyle.ScaleLineThickness         = 4.0f;
    gizmoStyle.ScaleLineCircleSize        = 7.0f;

    ImGuizmo::AllowAxisFlip(false);

    EditorThemeLoader::LoadTheme();
}

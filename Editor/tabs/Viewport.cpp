#include "Viewport.h"

#include "Editor.h"

#include "core/Renderer.h"
#include "core/Framebuffer.h"

Viewport::Viewport(Editor* owner) : EditorTab(owner)
{
    title  = "Viewport";
    flags |= ImGuiWindowFlags_NoScrollbar;
}

void Viewport::Tick()
{
    EditorTab::Tick();

    ImGui::SetNextWindowSize(ImVec2(static_cast<float>(width), static_cast<float>(height)));
    ImGui::SetNextWindowSizeConstraints(ImVec2(100, 100), ImVec2(2000, 2000));

    if (ImGui::Begin(title.c_str()))
    {
        const ImVec2 size  = ImGui::GetWindowContentRegionMax();

        width  = static_cast<int>(size.x);
        height = static_cast<int>(size.y);

        ImGui::End();
    }
}

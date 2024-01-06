#include "Viewport.h"

#include "Editor.h"

#include "core/Renderer.h"
#include "core/Framebuffer.h"

Viewport::Viewport(Editor* owner) : EditorTab(owner)
{
    title  = "Viewport";
    flags |= ImGuiWindowFlags_NoScrollbar;
    
    viewportFB = std::make_shared<Framebuffer>(1024, 768);
    viewportFB->Bind();
    
    width  = viewportFB->GetWidth();
    height = viewportFB->GetHeight();
}

void Viewport::Tick()
{
    EditorTab::Tick();

    ImGui::SetNextWindowSize(ImVec2(static_cast<float>(width), static_cast<float>(height)));
    ImGui::SetNextWindowSizeConstraints(ImVec2(100, 100), ImVec2(2000, 2000));

    ImGui::Begin(title.c_str());

    const ImVec2 size  = ImGui::GetWindowContentRegionMax();

    width  = static_cast<int>(size.x);
    height = static_cast<int>(size.y);

    viewportFB->Resize(width, height);
    Renderer::SetViewport(0, 0, width, height);

    const ImVec2 pos = ImGui::GetCursorScreenPos();
    ImGui::GetWindowDrawList()->AddImage((ImTextureID)viewportFB->GetTextureID(), pos, ImVec2(pos.x + width, pos.y + height), ImVec2(0, 1), ImVec2(1, 0));

    ImGui::End();
}

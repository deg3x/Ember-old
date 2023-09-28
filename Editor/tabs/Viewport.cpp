#include "Viewport.h"

#include "../Editor.h"
#include "core/Framebuffer.h"
#include "core/Window.h"

Viewport::Viewport(Editor* owner) : EditorTab(owner)
{
    title  = "Viewport";
    flags |= ImGuiWindowFlags_NoScrollbar;
    
    width  = editor->GetViewportFramebuffer()->GetWidth();
    height = editor->GetViewportFramebuffer()->GetHeight();
}

Viewport::~Viewport()
{
}

void Viewport::Tick()
{
    EditorTab::Tick();

    ImGui::SetNextWindowSizeConstraints(ImVec2(100, 100), ImVec2(2000, 2000));

    ImGui::Begin(title.c_str());
    
    /*const ImVec2 size  = ImGui::GetWindowContentRegionMax();

    width  = static_cast<int>(size.x);
    height = static_cast<int>(size.y);

    editor->GetViewportFramebuffer()->Resize(width, height);
    editor->GetEngineWindow()->SetGLViewport(width, height);*/
    

    ImVec2 pos = ImGui::GetCursorScreenPos();

    ImGui::GetWindowDrawList()->AddImage((ImTextureID)editor->GetViewportFramebuffer()->GetTextureID(), pos, ImVec2(pos.x + width, pos.y + height), ImVec2(0, 1), ImVec2(1, 0));

    ImGui::End();
}

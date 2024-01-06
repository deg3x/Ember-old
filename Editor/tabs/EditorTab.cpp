#include "EditorTab.h"

#include "Editor.h"
#include "core/Framebuffer.h"
#include "core/Renderer.h"

EditorTab::EditorTab(Editor* owner)
{
    editor = owner;
}

EditorTab::~EditorTab()
{
    editor = nullptr;
}

void EditorTab::Tick()
{
    ImGui::SetNextWindowSize(ImVec2(static_cast<float>(width), static_cast<float>(height)));
    
    ImGui::Begin(title.c_str());

    const ImVec2 size  = ImGui::GetWindowContentRegionMax();

    width  = static_cast<int>(size.x);
    height = static_cast<int>(size.y);

    editor->GetViewportFramebuffer()->Resize(width, height);
    Renderer::SetViewport(0, 0, width, height);

    ImGui::End();
}

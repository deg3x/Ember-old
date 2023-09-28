#include "EditorTab.h"

#include "../Editor.h"

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
    ImGui::End();
}

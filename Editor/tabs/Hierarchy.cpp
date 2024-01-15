#include "Hierarchy.h"

Hierarchy::Hierarchy(Editor* owner) : EditorTab(owner)
{
    title = "Hierarchy";
}

void Hierarchy::Tick()
{
    EditorTab::Tick();

    if (ImGui::Begin(title.c_str()))
    {
        ImGui::Text("This is the hierarchy window");
        
        ImGui::End();
    }
}

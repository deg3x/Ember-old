#include "Inspector.h"

Inspector::Inspector(Editor* owner) : EditorTab(owner)
{
    title = "Inspector";
}

void Inspector::Tick()
{
    EditorTab::Tick();

    if (ImGui::Begin(title.c_str()))
    {
        ImGui::Text("This is the inspector window");
        
        ImGui::End();
    }
}

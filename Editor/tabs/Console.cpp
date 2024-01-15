#include "Console.h"

Console::Console(Editor* owner) : EditorTab(owner)
{
    title = "Console";
}

void Console::Tick()
{
    EditorTab::Tick();

    if (ImGui::Begin(title.c_str()))
    {
        ImGui::Text("This is the console window");
        
        ImGui::End();
    }
}

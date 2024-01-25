#include "editor_pch.h"
#include "MainMenuBar.h"

MainMenuBar::MainMenuBar(Editor* owner) : EditorTab(owner)
{
    title = "MainMenu";
}

void MainMenuBar::Tick()
{
    EditorTab::Tick();

    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("Editor"))
        {
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Help"))
        {
            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }
}

#pragma once

#include "imgui/imgui.h"

enum class TabType
{
    VIEWPORT,
    HIERARCHY,
    INSPECTOR,
    CONSOLE,
    MAIN_MENU_BAR,
    NONE
};

class Editor;

class EditorTab
{
protected:
    Editor* editor;  

    std::string title = "Unnamed Tab";
    int flags         = ImGuiWindowFlags_NoCollapse;

    float width;
    float height;
    
public:
    EditorTab() = delete;
    EditorTab(Editor* owner);
    virtual ~EditorTab();

    virtual void Tick() {}

    virtual TabType GetType() const
    {
        return TabType::NONE;
    }
};

#pragma once

#include <string>

#include "../imgui/imgui.h"

class Editor;

class EditorTab
{
protected:
    Editor* editor;  

    std::string title = "Unnamed Tab";
    int flags         = ImGuiWindowFlags_NoCollapse;

    int width;
    int height;
    
public:
    EditorTab() = delete;
    EditorTab(Editor* owner);
    virtual ~EditorTab();

    virtual void Tick() {}
};

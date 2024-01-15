#pragma once

#include <memory>
#include <vector>

#include "imgui/imgui.h"

class EditorTab;

class Editor
{
private:
    std::vector<std::shared_ptr<EditorTab>> tabs;
    ImGuiWindowFlags windowFlags;
    
public:
    Editor();
    virtual ~Editor();

    void Tick();

private:
    void EditorRenderBegin();
    void EditorRenderEnd();
};

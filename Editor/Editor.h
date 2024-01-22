#pragma once

#include <memory>
#include <vector>

#include "tabs/EditorTab.h"
#include "imgui/imgui.h"

class Editor
{
private:
    std::vector<std::shared_ptr<EditorTab>> tabs;
    ImGuiWindowFlags windowFlags;
    
public:
    Editor();
    virtual ~Editor();

    void Tick();

    std::shared_ptr<EditorTab> FindTabByType(TabType type) const;

private:
    void RenderEditor() const;
};

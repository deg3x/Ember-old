#pragma once

#include <memory>
#include <vector>

class Window;
class EditorTab;

class Editor
{
private:
    Window* engineWindow;

    std::vector<std::unique_ptr<EditorTab>> tabs;
    
public:
    Editor();
    virtual ~Editor();

    void Tick();
};

#pragma once

#include <memory>
#include <vector>

class EditorTab;

class Editor
{
private:
    std::vector<std::shared_ptr<EditorTab>> tabs;
    
public:
    Editor();
    virtual ~Editor();

    void Tick();
};

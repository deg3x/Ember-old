#pragma once

#include <memory>
#include <vector>

class Framebuffer;
class EditorTab;

class Editor
{
private:
    std::shared_ptr<Framebuffer> viewportFB;

    std::vector<std::shared_ptr<EditorTab>> tabs;
    
public:
    Editor();
    virtual ~Editor();

    void Tick();

    inline std::shared_ptr<Framebuffer> GetViewportFramebuffer() const
    {
        return viewportFB;
    }
};

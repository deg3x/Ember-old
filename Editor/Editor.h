#pragma once

#include <memory>
#include <vector>

class Framebuffer;
class Window;
class EditorTab;

class Editor
{
private:
    std::shared_ptr<Window> engineWindow;
    std::shared_ptr<Framebuffer> viewportFB;

    std::vector<std::shared_ptr<EditorTab>> tabs;
    
public:
    Editor();
    virtual ~Editor();

    void Tick();

    inline std::shared_ptr<Window> GetEngineWindow() const
    {
        return engineWindow;
    }
    inline std::shared_ptr<Framebuffer> GetViewportFramebuffer() const
    {
        return viewportFB;
    }
};

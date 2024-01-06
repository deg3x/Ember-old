#pragma once

#include "EditorTab.h"

#include <memory>

class Framebuffer;

class Viewport : public EditorTab
{
private:
    std::shared_ptr<Framebuffer> viewportFB;
    
public:
    Viewport() = delete;
    Viewport(Editor* owner);
    virtual ~Viewport() override = default;
    
    void Tick() override;
    
    inline std::shared_ptr<Framebuffer> GetViewportFramebuffer() const
    {
        return viewportFB;
    }
};

#pragma once

#include "EditorTab.h"
#include "core/Scene.h"
#include "core/Framebuffer.h"

#include <memory>

class Framebuffer;

class Viewport : public EditorTab
{
private:
    Scene viewportScene;
    std::unique_ptr<Framebuffer> viewportFB = nullptr;
    
public:
    Viewport() = delete;
    Viewport(Editor* owner);
    virtual ~Viewport() override = default;
    
    void Tick() override;
};

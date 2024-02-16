#pragma once
#include "EditorTab.h"

class Object;
class FrameBuffer;

class Viewport : public EditorTab
{
public:
    inline static constexpr float cameraRotSpeed  = 5.0f;
    inline static constexpr float cameraZoomSpeed = 3.0f;

private:
    std::shared_ptr<Object> viewportCamera;
    
public:
    Viewport() = delete;
    Viewport(Editor* owner);
    virtual ~Viewport() override = default;
    
    void Tick() override;

    TabType GetType() const override
    {
        return TabType::VIEWPORT;
    }

private:
    void TickViewportCamera();
};

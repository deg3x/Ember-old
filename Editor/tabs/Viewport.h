#pragma once
#include "EditorTab.h"

class FrameBuffer;

class Viewport : public EditorTab
{
public:
    inline static float cameraRotSpeed  = 5.0f;
    inline static float cameraZoomSpeed = 8.0f;
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

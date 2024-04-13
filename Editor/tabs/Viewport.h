#pragma once
#include "EditorTab.h"

class Object;
class FrameBuffer;

class Viewport : public EditorTab
{
public:
    inline static constexpr float CameraFreeSpeedRot = 3.0f;
    inline static constexpr float CameraFreeSpeedMod = 1.0f;
    inline static constexpr float CameraOrbitSpeed   = 5.0f;
    inline static constexpr float CameraZoomSpeed    = 3.0f;
    
    inline static float CameraFreeSpeedMove = 5.0f;

private:
    std::shared_ptr<Object> viewportCamera;
    glm::vec3 cameraFocus = glm::vec3(0.0f, 0.0f, 0.0f);
    
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
    void TickGuizmo();
    void TickViewportCamera();
    void CameraFreeMove();
    void CameraOrbit();
    void CameraZoom();
};

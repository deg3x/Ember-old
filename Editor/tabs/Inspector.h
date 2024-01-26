#pragma once

#include "EditorTab.h"

class Camera;
class Hierarchy;

class Inspector : public EditorTab
{
private:
    std::shared_ptr<Hierarchy> hierarchyTab;

    static constexpr ImVec4 labelColorX = {0.75f, 0.20f, 0.20f, 0.80f};
    static constexpr ImVec4 labelColorY = {0.20f, 0.75f, 0.20f, 0.80f};
    static constexpr ImVec4 labelColorZ = {0.20f, 0.20f, 0.75f, 0.80f};

    static constexpr float componentColFirstWidth  = 0.25f;
    static constexpr float componentColSecondWidth = 0.75f;
    static constexpr float componentLabelIndent    = 0.2f;
    static constexpr float componentDragFloatSpeed = 0.1f;

public:
    Inspector() = delete;
    Inspector(Editor* owner);
    virtual ~Inspector() override = default;

    void Tick() override;

    TabType GetType() const override
    {
        return TabType::INSPECTOR;
    }

private:
    void DrawComponents();
    void DrawTransform();
    void DrawTransformVector(const std::string& label, glm::vec3* vector);
    void DrawCamera(const std::shared_ptr<Camera>& cameraComponent);
    void DrawCameraElements(const std::shared_ptr<Camera>& cameraComponent);
    void DrawLight();
    void DrawMesh();
};

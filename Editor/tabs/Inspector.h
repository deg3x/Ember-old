#pragma once

#include "EditorTab.h"

class Mesh;
class Light;
class Camera;
class Hierarchy;

class Inspector : public EditorTab
{
private:
    std::shared_ptr<Hierarchy> hierarchyTab;

    static constexpr ImVec4 labelColorX = {0.75f, 0.20f, 0.20f, 0.80f};
    static constexpr ImVec4 labelColorY = {0.20f, 0.75f, 0.20f, 0.80f};
    static constexpr ImVec4 labelColorZ = {0.20f, 0.20f, 0.75f, 0.80f};

    static constexpr float compColWidthFirstTransf  = 0.2f;
    static constexpr float compColWidthSecondTransf = 0.8f;
    static constexpr float compColWidthFirst        = 0.35f;
    static constexpr float compColWidthSecond       = 0.65f;
    static constexpr float compLabelIndent          = 0.25f;
    static constexpr float compDragFloatSpeed       = 0.1f;

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
    void DrawLight(const std::shared_ptr<Light>& lightComponent);
    void DrawLightElements(const std::shared_ptr<Light>& lightComponent);
    void DrawMesh(const std::shared_ptr<Mesh>& meshComponent);
    void DrawMeshElements(const std::shared_ptr<Mesh>& meshComponent);
    void DrawAddComponentButton();

    void DrawRowLabelDragFloat(const std::string& label, float& target);
    void DrawRowLabelColor3(const std::string& label, float target[3]);
    void DrawRowLabelText(const std::string& label, const std::string& text);
};

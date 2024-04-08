#pragma once

#include "EditorTab.h"

class Material;
class Mesh;
class Light;
class Camera;
class Hierarchy;

class Inspector : public EditorTab
{
private:
    std::shared_ptr<Hierarchy> hierarchyTab;

    static constexpr float compColWidthFirstTransf  = 0.2f;
    static constexpr float compColWidthSecondTransf = 0.8f;
    static constexpr float compColWidthFirst        = 0.40f;
    static constexpr float compColWidthSecond       = 0.60f;
    static constexpr float compColWidthSecondMult   = 0.9f;
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
    void DrawHeader();
    void DrawComponents();
    void DrawTransform();
    void DrawTransformVector(const std::string& label, glm::vec3* vector);
    void DrawCamera(const std::shared_ptr<Camera>& cameraComponent);
    void DrawCameraElements(const std::shared_ptr<Camera>& cameraComponent);
    void DrawLight(const std::shared_ptr<Light>& lightComponent);
    void DrawLightElements(const std::shared_ptr<Light>& lightComponent);
    void DrawMesh(const std::shared_ptr<Mesh>& meshComponent);
    void DrawMeshElements(const std::shared_ptr<Mesh>& meshComponent);
    void DrawMaterial(const std::shared_ptr<Material>& material);
    void DrawMaterialElements(const std::shared_ptr<Material>& material);
    void DrawAddComponentButton();

    void DrawRowLabel(const std::string& label);
    void DrawRowLabelCheckbox(const std::string& label, bool& target);
    void DrawRowLabelDragInt(const std::string& label, int& target);
    void DrawRowLabelDragFloat(const std::string& label, float& target);
    void DrawRowLabelDragFloat3(const std::string& label, glm::vec3& target);
    void DrawRowLabelDragFloat4(const std::string& label, glm::vec4& target);
    void DrawRowLabelColor3(const std::string& label, float target[3]);
    void DrawRowLabelText(const std::string& label, const std::string& text);
    void DrawRowLabelTexture(const std::string& label, unsigned int id);
};

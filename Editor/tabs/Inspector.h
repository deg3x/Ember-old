#pragma once

#include <memory>
#include <glm/vec3.hpp>

#include "EditorTab.h"

class Hierarchy;

class Inspector : public EditorTab
{
private:
    std::shared_ptr<Hierarchy> hierarchyTab;

    static constexpr ImVec4 labelColorX = {0.75f, 0.20f, 0.20f, 0.80f};
    static constexpr ImVec4 labelColorY = {0.20f, 0.75f, 0.20f, 0.80f};
    static constexpr ImVec4 labelColorZ = {0.20f, 0.20f, 0.75f, 0.80f};

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
    void DrawTransform();
    void DrawTransformVector(const std::string& label, glm::vec3* vector);
    void DrawDragFloatLabel(const std::string& label);
};

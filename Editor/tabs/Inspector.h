#pragma once

#include <memory>
#include <glm/vec3.hpp>

#include "EditorTab.h"

class Hierarchy;

class Inspector : public EditorTab
{
private:
    std::shared_ptr<Hierarchy> hierarchyTab; 

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
};

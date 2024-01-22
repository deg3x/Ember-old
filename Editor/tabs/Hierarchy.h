#pragma once
#include "EditorTab.h"

#include <memory>

class Object;

class Hierarchy : public EditorTab
{
private:
    std::weak_ptr<Object> selectedObject;
    std::weak_ptr<Object> hoveredObject;

public:
    Hierarchy() = delete;
    Hierarchy(Editor* owner);
    virtual ~Hierarchy() override = default;

    void Tick() override;
};

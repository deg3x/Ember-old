#pragma once
#include "EditorTab.h"

#include <memory>

class Object;

class Hierarchy : public EditorTab
{
public:
    std::weak_ptr<Object> SelectedObject;
    
private:
    std::weak_ptr<Object> hoveredObject;
    std::weak_ptr<Object> clickedObject;

    static constexpr ImVec4 itemColor        = {0.10f, 0.55f, 0.45f, 1.00f};
    static constexpr ImVec4 itemColorHovered = {0.10f, 0.55f, 0.45f, 0.75f};
    static constexpr ImVec4 itemColorActive  = {0.10f, 0.55f, 0.45f, 0.40f};

public:
    Hierarchy() = delete;
    Hierarchy(Editor* owner);
    virtual ~Hierarchy() override = default;

    void Tick() override;

    TabType GetType() const override
    {
        return TabType::HIERARCHY;
    }

private:
    void HandleMouseBehavior();
};

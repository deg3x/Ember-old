#pragma once
#include "EditorTab.h"

class Hierarchy : public EditorTab
{
public:
    Hierarchy() = delete;
    Hierarchy(Editor* owner);
    virtual ~Hierarchy() override = default;

    void Tick() override;
};

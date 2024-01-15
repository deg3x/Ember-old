#pragma once
#include "EditorTab.h"

class Inspector : public EditorTab
{
public:
    Inspector() = delete;
    Inspector(Editor* owner);
    virtual ~Inspector() override = default;

    void Tick() override;
};

#pragma once
#include "EditorTab.h"

class Console : public EditorTab
{
public:
    Console() = delete;
    Console(Editor* owner);
    virtual ~Console() override = default;

    void Tick() override;
};

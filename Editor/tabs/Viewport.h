#pragma once

#include "EditorTab.h"

class Viewport : public EditorTab
{
public:
    Viewport() = delete;
    Viewport(Editor* owner);
    virtual ~Viewport();
    
    void Tick() override;
};

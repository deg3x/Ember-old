#pragma once
#include "EditorTab.h"

class MainMenuBar : public EditorTab
{
public:
    MainMenuBar() = delete;
    MainMenuBar(Editor* owner);
    virtual ~MainMenuBar() override = default;

    void Tick() override;
};

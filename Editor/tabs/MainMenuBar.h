#pragma once
#include "EditorTab.h"

class MainMenuBar : public EditorTab
{
public:
    MainMenuBar() = delete;
    MainMenuBar(Editor* owner);
    virtual ~MainMenuBar() override = default;

    void Tick() override;
    
    TabType GetType() const override
    {
        return TabType::MAIN_MENU_BAR;
    }
};

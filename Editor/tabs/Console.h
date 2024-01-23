#pragma once
#include "EditorTab.h"

class Console : public EditorTab
{
private:
    bool showInfo = true;
    bool showWarning = true;
    bool showError = true;
    
public:
    Console() = delete;
    Console(Editor* owner);
    virtual ~Console() override = default;

    void Tick() override;

    TabType GetType() const override
    {
        return TabType::CONSOLE;
    }

private:
    void DrawCategoryButtons();
    void DrawConsoleContent() const;
};

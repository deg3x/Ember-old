#pragma once
#include "EditorTab.h"

class Console : public EditorTab
{
private:
    bool showInfo = true;
    bool showWarning = true;
    bool showError = true;

    ImVec4 textColorInfo;
    ImVec4 textColorWarning;
    ImVec4 textColorError;

    ImVec4 buttonColorCategory;
    ImVec4 buttonColorHoverCategory;
    
public:
    Console() = delete;
    Console(Editor* owner);
    virtual ~Console() override = default;

    void Tick() override;

private:
    void DrawCategoryButtons();
    void DrawConsoleContent() const;
};

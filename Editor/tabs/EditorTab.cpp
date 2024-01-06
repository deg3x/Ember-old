#include "EditorTab.h"

#include "Editor.h"

EditorTab::EditorTab(Editor* owner)
{
    editor = owner;
}

EditorTab::~EditorTab()
{
    editor = nullptr;
}

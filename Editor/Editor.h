#pragma once

class Window;

class Editor
{
private:
    Window* mainWindow;
    
public:
    Editor();
    virtual ~Editor();

    void Tick();
};

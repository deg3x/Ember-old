#pragma once

class Window;

class Editor
{
private:
    Window* engineWindow;
    
public:
    Editor();
    virtual ~Editor();

    void Tick();
};

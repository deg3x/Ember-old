#include "Engine.h"

#include "window/Window.h"
#include "core/World.h"
#include "core/Renderer.h"
#include "core/Time.h"
#include "input/Input.h"

void Engine::Initialize()
{
    Window::Initialize();
    Time::Initialize();
    Renderer::Initialize();
    Input::Initialize();
    World::Initialize();
}

void Engine::Tick()
{
    Time::Tick();
    Input::Tick();
    World::Tick();
}

#include "Engine.h"

#include "window/Window.h"
#include "core/Renderer.h"
#include "input/Input.h"

void Engine::Initialize()
{
    Window::Initialize();
    Renderer::Initialize();
    Input::Initialize();
}

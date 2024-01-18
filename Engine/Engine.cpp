#include "Engine.h"

#include "core/Window.h"
#include "core/Renderer.h"
#include "input/Input.h"

void Engine::Initialize()
{
    Window::Initialize();
    Renderer::Initialize();
    Input::Initialize();
}

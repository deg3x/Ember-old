#include "engine_pch.h"
#include "Engine.h"

#include "core/FrameBuffer.h"
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
    Renderer::ViewportFrameBuffer->Bind();
    Renderer::Clear();
    
    Time::Tick();
    Input::Tick();
    World::Tick();
    Renderer::Tick();
    
    Renderer::ViewportFrameBuffer->Unbind();
}

#include "editor_pch.h"
#include "Viewport.h"

#include "glad/glad.h"

#include "Editor.h"

#include "core/World.h"
#include "core/Renderer.h"
#include "core/Framebuffer.h"
#include "core/Object.h"
#include "core/components/Camera.h"
#include "core/components/Transform.h"

#include "input/Input.h"

namespace
{
    float theta  = -glm::half_pi<float>();
    float phi    = -glm::half_pi<float>();
    float radius;
	
    ImVec4 clear_color = ImVec4(0.16f, 0.15f, 0.18f, 1.00f);
}

Viewport::Viewport(Editor* owner) : EditorTab(owner)
{
    title  = "Viewport";
    flags |= ImGuiWindowFlags_NoScrollbar;
    flags |= ImGuiWindowFlags_NoMove;

    // Initialization of viewport scene
    radius = (float)World::GetCamera()->GetOwner()->transform->position.length();
    
    Renderer::SetDepthTestEnabled(true);
    Renderer::SetDepthTestFunc(GL_LESS);
    Renderer::SetBlendingEnabled(true);
    Renderer::SetBlendingFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE);
    Renderer::SetBlendingOp(GL_FUNC_ADD);
    Renderer::SetClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
}

void Viewport::Tick()
{
    EditorTab::Tick();

    ImGui::SetNextWindowSize(ImVec2(width, height));
    ImGui::SetNextWindowSizeConstraints(ImVec2(100, 100), ImVec2(2000, 2000));

    if (!ImGui::Begin(title.c_str(), nullptr, flags))
    {
        return;
    }

    const ImVec2 viewportSize = ImGui::GetContentRegionAvail();

    width  = viewportSize.x;
    height = viewportSize.y;
    
    Renderer::SetViewport(0, 0, static_cast<int>(width), static_cast<int>(height));

    if (ImGui::IsWindowHovered())
    {
        const MouseData mouse = Input::Mouse;

        if (Input::GetMouseDrag(MOUSE_BTN_LEFT))
        {
            theta  += (float)mouse.leftMouseDragDeltaX * mouse.sensitivity;
            phi	   += (float)mouse.leftMouseDragDeltaY * mouse.sensitivity;
        }
        if (Input::GetMouseDrag(MOUSE_BTN_RIGHT))
        {
            radius -= (float)mouse.rightMouseDragDeltaY * mouse.sensitivity;
        }

        World::GetCamera()->GetOwner()->transform->position.x = radius * glm::cos(theta) * glm::sin(phi);
        World::GetCamera()->GetOwner()->transform->position.z = radius * glm::sin(theta) * glm::sin(phi);
        World::GetCamera()->GetOwner()->transform->position.y = radius * glm::cos(phi);
    }

    ImGui::Image(reinterpret_cast<ImTextureID>(Renderer::WorldFrameBuffer->GetTextureID()), viewportSize, ImVec2(0, 1), ImVec2(1, 0));
    ImGui::End();
}

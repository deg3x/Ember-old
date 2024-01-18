#include "Viewport.h"

#include <glad/glad.h>
#include <glm/gtc/constants.hpp>

#include "Editor.h"

#include "core/Renderer.h"
#include "core/Framebuffer.h"
#include "core/objects/Object.h"
#include "core/components/Camera.h"
#include "core/components/Transform.h"

#include "input/Input.h"
#include "logger/Logger.h"

namespace
{
    float theta  = -glm::quarter_pi<float>();
    float phi    = -glm::half_pi<float>();
    float radius;
	
    ImVec4 clear_color = ImVec4(0.16f, 0.15f, 0.18f, 1.00f);
}

Viewport::Viewport(Editor* owner) : EditorTab(owner)
{
    title  = "Viewport";
    flags |= ImGuiWindowFlags_NoScrollbar;

    // Initialization of viewport scene
    viewportScene.Use();
    radius = (float)viewportScene.GetCamera()->GetParent()->transform->position.length();
    
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

    if (!ImGui::Begin(title.c_str()))
    {
        return;
    }

    const ImVec2 viewportSize = ImGui::GetContentRegionAvail();

    width  = viewportSize.x;
    height = viewportSize.y;

    if (viewportFB == nullptr)
    {
        viewportFB = std::make_unique<Framebuffer>(static_cast<int>(width), static_cast<int>(height));
    }
    else
    {
        viewportFB->Resize(static_cast<int>(width), static_cast<int>(height));
    }
    
    Renderer::SetViewport(0, 0, static_cast<int>(width), static_cast<int>(height));

    Input::PollInput();

    if (ImGui::IsWindowHovered())
    {
        const MouseData mouse = Input::Mouse;

        theta  += (float)mouse.leftMouseDragDeltaX * mouse.sensitivity;
        phi	   += (float)mouse.leftMouseDragDeltaY * mouse.sensitivity;
        radius -= (float)mouse.rightMouseDragDeltaY * mouse.sensitivity;

        viewportScene.GetCamera()->GetParent()->transform->position.x = radius * glm::cos(theta) * glm::sin(phi);
        viewportScene.GetCamera()->GetParent()->transform->position.z = radius * glm::sin(theta) * glm::sin(phi);
        viewportScene.GetCamera()->GetParent()->transform->position.y = radius * glm::cos(phi);
    }
    
    viewportFB->Bind();

    Renderer::Clear();
    viewportScene.Tick();

    viewportFB->Unbind();

    ImGui::Image(reinterpret_cast<ImTextureID>(viewportFB->GetTextureID()), viewportSize, ImVec2(0, 1), ImVec2(1, 0));

    ImGui::End();
}

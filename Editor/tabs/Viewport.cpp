#include "editor_pch.h"
#include "Viewport.h"

#include "Editor.h"

#include "core/World.h"
#include "core/Renderer.h"
#include "core/Object.h"
#include "core/Time.h"
#include "core/components/Camera.h"
#include "core/components/Transform.h"
#include "input/Input.h"

Viewport::Viewport(Editor* owner) : EditorTab(owner)
{
    title  = "Viewport";
    flags |= ImGuiWindowFlags_NoScrollbar;
    flags |= ImGuiWindowFlags_NoMove;
    
    viewportCamera = std::make_shared<Object>("Camera");
    Camera::ActiveCamera = viewportCamera->CreateComponent<Camera>();
    viewportCamera->transform->SetPosition({0.0f, 3.0f, 3.0f});
    viewportCamera->transform->SetRotation({0.0f, -90.0f, 0.0f});

    World::AddObject(viewportCamera);
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

    if (ImGui::IsWindowHovered() && ImGui::IsWindowFocused())
    {
        TickViewportCamera();
    }

    ImGui::Image(reinterpret_cast<ImTextureID>(Renderer::GetViewportTextureID()), viewportSize, ImVec2(0, 1), ImVec2(1, 0));
    ImGui::End();
}

void Viewport::TickViewportCamera()
{
    const MouseData mouse = Input::Mouse;

    const std::shared_ptr<Transform> cameraTransform = viewportCamera->transform;
    
    glm::vec3 newPosition = cameraTransform->GetPosition();

    // Rotation around center
    const float rotSpeed     = mouse.sensitivity * Time::DeltaTime * cameraRotSpeed;
    const float angleYaw     = static_cast<float>(mouse.leftMouseDragDeltaX) * rotSpeed * -1.0f;
    const float anglePitch   = static_cast<float>(mouse.leftMouseDragDeltaY) * rotSpeed;
    const float cosHalfYaw   = glm::cos(angleYaw * 0.5f);
    const float sinHalfYaw   = glm::sin(angleYaw * 0.5f);
    const float cosHalfPitch = glm::cos(anglePitch * 0.5f);
    const float sinHalfPitch = glm::sin(anglePitch * 0.5f);
    
    const glm::vec3 upVector    = glm::vec3(0.0f, 1.0f, 0.0f);
    const glm::vec3 rightVector = normalize(glm::cross(cameraTransform->GetPosition(), upVector));
    
    const glm::quat rotationYaw   = glm::quat(cosHalfYaw, upVector * sinHalfYaw);
    const glm::quat rotationPitch = glm::quat(cosHalfPitch, rightVector * sinHalfPitch);
    
    bool applyYaw   = Input::GetMouseDrag(MOUSE_BTN_LEFT);
    bool applyPitch = Input::GetMouseDrag(MOUSE_BTN_LEFT);
    applyPitch     &= glm::abs(glm::dot(upVector, glm::normalize(rotationPitch * cameraTransform->GetPosition()))) < 0.99f;
    
    newPosition = applyYaw ? rotationYaw * newPosition : newPosition;
    newPosition = applyPitch ? rotationPitch * newPosition : newPosition;

    // Distance from center
    constexpr float minZoomDistance  = 1.0f;
    constexpr float minDistanceSpeed = 5.0f;
    constexpr float maxDistanceSpeed = 150.0f;
    constexpr float minDistanceMult  = 1.0f;
    constexpr float maxDistanceMult  = 5.0f;
    
    const float speedDistMultDelta = glm::clamp((glm::length(cameraTransform->GetPosition()) - minDistanceSpeed) / (maxDistanceSpeed - minDistanceSpeed), 0.0f, 1.0f); 
    const float speedDistanceMult  = speedDistMultDelta * maxDistanceMult + (1.0f - speedDistMultDelta) * minDistanceMult;
        
    const float zoomSpeed         = mouse.sensitivityScroll * Time::DeltaTime * cameraZoomSpeed * speedDistanceMult;
    const float distanceDelta     = static_cast<float>(mouse.mouseScrollDeltaYSmooth) * zoomSpeed * -1.0f;
    const glm::vec3 zoomDirection = glm::normalize(newPosition);
    
    bool applyDistance = glm::length(newPosition + zoomDirection * distanceDelta) > minZoomDistance;
    
    newPosition = newPosition + (applyDistance ? zoomDirection * distanceDelta : glm::vec3(0.0));

    // Apply new camera position
    cameraTransform->SetPosition(newPosition);
}

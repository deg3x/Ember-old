#include "editor_pch.h"
#include "Viewport.h"

#include "Editor.h"
#include "Hierarchy.h"

#include "core/World.h"
#include "core/Renderer.h"
#include "core/Object.h"
#include "core/Time.h"
#include "core/components/Camera.h"
#include "core/components/Transform.h"
#include "imgui/ImGuizmo/ImGuizmo.h"
#include "input/Input.h"
#include "input/KeyCodes.h"
#include "logger/Logger.h"
#include "themes/EditorTheme.h"

Viewport::Viewport(Editor* owner) : EditorTab(owner)
{
    title  = "Viewport";
    flags |= ImGuiWindowFlags_NoScrollbar;
    flags |= ImGuiWindowFlags_NoMove;
    
    viewportCamera = std::make_shared<Object>("Camera");
    Camera::ActiveCamera = viewportCamera->CreateComponent<Camera>();
    viewportCamera->transform->SetPosition({3.0f, 2.0f, 4.5f});
    viewportCamera->transform->SetRotationEuler({-160.0f, -30.0f, 180.0f});

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
    
    if (ImGui::IsWindowHovered() && ImGui::IsWindowFocused() && !ImGuizmo::IsOver() && !ImGuizmo::IsUsing())
    {
        TickViewportCamera();
    }

    ImGui::Image(reinterpret_cast<ImTextureID>(Renderer::GetViewportTextureID()), viewportSize, ImVec2(0, 1), ImVec2(1, 0));

    TickGuizmo();
    
    ImGui::End();
}

void Viewport::TickGuizmo()
{
    static ImGuizmo::OPERATION operation = ImGuizmo::OPERATION::TRANSLATE;
    static ImGuizmo::MODE mode           = ImGuizmo::MODE::WORLD;
    static float snapValues[3];

    if (Input::GetKey(KEYCODE_W))
    {
        operation = ImGuizmo::OPERATION::TRANSLATE;
    }
    else if (Input::GetKey(KEYCODE_E))
    {
        operation = ImGuizmo::OPERATION::ROTATE;
    }
    else if (Input::GetKey(KEYCODE_R))
    {
        operation = ImGuizmo::OPERATION::SCALE;
    }

    switch(operation)
    {
    case ImGuizmo::TRANSLATE:
        ImGuizmo::SetGizmoSizeClipSpace(EditorTheme::ViewportGizmoSizeT);
        snapValues[0] = 0.10f;
        snapValues[1] = 0.10f;
        snapValues[2] = 0.10f;
        break;
    case ImGuizmo::ROTATE:
        ImGuizmo::SetGizmoSizeClipSpace(EditorTheme::ViewportGizmoSizeR);
        snapValues[0] = 1.00f;
        snapValues[1] = 1.00f;
        snapValues[2] = 1.00f;
        break;
    case ImGuizmo::SCALE:
        ImGuizmo::SetGizmoSizeClipSpace(EditorTheme::ViewportGizmoSizeS);
        snapValues[0] = 0.10f;
        snapValues[1] = 0.10f;
        snapValues[2] = 0.10f;
        break;
    default:
        ImGuizmo::SetGizmoSizeClipSpace(EditorTheme::ViewportGizmoSizeT);
    }
    
    const ImVec2 windowPos = ImGui::GetWindowPos();
    const float windowW    = ImGui::GetWindowWidth();
    const float windowH    = ImGui::GetWindowHeight();

    glm::mat4x4 view = Camera::ActiveCamera->GetViewMatrix();
    glm::mat4x4 proj = Camera::ActiveCamera->GetProjectionMatrix();
    glm::mat4x4 id   = glm::mat4x4(1.0f);
    
    const std::shared_ptr<Hierarchy> hierarchyTab = std::dynamic_pointer_cast<Hierarchy>(editor->FindTabByType(TabType::HIERARCHY));
    const std::shared_ptr<Object> selected = hierarchyTab->SelectedObject.lock();

    ImGuizmo::SetDrawlist();
    ImGuizmo::SetRect(windowPos.x, windowPos.y, windowW, windowH);
    
    if (selected)
    {
        const bool isOrthographic = Camera::ActiveCamera->projectionType == CameraProjection::ORTHOGRAPHIC;
        ImGuizmo::SetOrthographic(isOrthographic);

        glm::mat4x4 model = selected->transform->GetModelMatrix();
        glm::mat4x4 delta;

        const bool shouldSnap = Input::GetKey(KEYCODE_LEFT_CONTROL);
        bool isModified = ImGuizmo::Manipulate(&view[0][0], &proj[0][0], operation, mode, &model[0][0], &delta[0][0], shouldSnap ? &snapValues[0] : nullptr);

        if (ImGuizmo::IsUsing() && isModified)
        {
            glm::vec3 position;
            glm::vec3 rotation;
            glm::vec3 scale;

            ImGuizmo::DecomposeMatrixToComponents(&delta[0][0], &position.x, &rotation.x, &scale.x);
            
            switch(operation)
            {
            case ImGuizmo::TRANSLATE:
                selected->transform->Translate(position);
                break;
            case ImGuizmo::ROTATE:
                glm::quat rotQuat(glm::radians(rotation));
                selected->transform->Rotate(rotQuat);
                break;
            case ImGuizmo::SCALE:
                selected->transform->Scale(scale);
                break;
            default:
                break;
            }
        }
    }

    const ImGuiStyle style = ImGui::GetStyle();

    constexpr float offsetX    = 20.0f;
    constexpr float offsetY    = 10.0f;
    const float cubeDimension  = glm::clamp(windowW * 0.07f, 50.0f, 200.0f);
    const ImVec2 viewGizmoSize = {cubeDimension, cubeDimension};
    const ImVec2 viewGizmoPos  = {windowW - viewGizmoSize.x - offsetX, windowPos.y + 4 * style.FramePadding.y + ImGui::CalcTextSize("Viewport").y + offsetY};
        
    ImGuizmo::ViewManipulate(&view[0][0], &proj[0][0], operation, mode, &id[0][0], 1.0f, viewGizmoPos, viewGizmoSize, IM_COL32(0, 0, 0, 0));

    // DECOMPOSE AND USE THE VIEW MANIPULATION
}

void Viewport::TickViewportCamera()
{
    const MouseData mouse = Input::Mouse;

    const std::shared_ptr<Transform> cameraTransform = viewportCamera->transform;

    static glm::vec3 focusPoint = glm::vec3(0.0f, 0.0f, 0.0f);
    static float moveSpeed = 5.0f;
    glm::vec3 newPosition = cameraTransform->GetPosition();
    
    const float rotSpeed = mouse.sensitivity * Time::DeltaTime * cameraRotSpeed;

    if (Input::GetMouse(MOUSE_BTN_RIGHT))
    {
        glm::vec3 offset = glm::vec3(0.0f, 0.0f, 0.0f);
        
        if (Input::GetKey(KEYCODE_W))
        {
            offset += cameraTransform->GetForwardVector() * moveSpeed * Time::DeltaTime;
        }
        if (Input::GetKey(KEYCODE_D))
        {
            offset += cameraTransform->GetRightVector() * moveSpeed * Time::DeltaTime;
        }
        if (Input::GetKey(KEYCODE_E))
        {
            offset += cameraTransform->GetUpVector() * moveSpeed * Time::DeltaTime;
        }
        if (Input::GetKey(KEYCODE_S))
        {
            offset -= cameraTransform->GetForwardVector() * moveSpeed * Time::DeltaTime;
        }
        if (Input::GetKey(KEYCODE_A))
        {
            offset -= cameraTransform->GetRightVector() * moveSpeed * Time::DeltaTime;
        }
        if (Input::GetKey(KEYCODE_Q))
        {
            offset -= cameraTransform->GetUpVector() * moveSpeed * Time::DeltaTime;
        }

        focusPoint += offset;
        cameraTransform->Translate(offset);

        const float angleYaw     = static_cast<float>(mouse.rightMouseDragDeltaX) * rotSpeed * -1.0f;
        const float anglePitch   = static_cast<float>(mouse.rightMouseDragDeltaY) * rotSpeed;
        const float cosHalfYaw   = glm::cos(angleYaw * 0.5f);
        const float sinHalfYaw   = glm::sin(angleYaw * 0.5f);
        const float cosHalfPitch = glm::cos(anglePitch * 0.5f);
        const float sinHalfPitch = glm::sin(anglePitch * 0.5f);

        const glm::vec3 rightVector = cameraTransform->GetRightVector();
    
        const glm::quat rotationYaw   = glm::quat(cosHalfYaw, Transform::WorldUp * sinHalfYaw);
        const glm::quat rotationPitch = glm::quat(cosHalfPitch, -rightVector * sinHalfPitch);
    
        bool applyYaw   = Input::GetMouseDrag(MOUSE_BTN_RIGHT);
        bool applyPitch = Input::GetMouseDrag(MOUSE_BTN_RIGHT);
        applyPitch     &= glm::abs(glm::dot(Transform::WorldUp, glm::normalize(rotationPitch * cameraTransform->GetPosition()))) < 0.99f;

        if (applyYaw)
        {
            cameraTransform->Rotate(rotationYaw);
        }
        if (applyPitch)
        {
            cameraTransform->Rotate(rotationPitch);
        }
    }
    else if (Input::GetKey(KEYCODE_LEFT_ALT))
    {
        const float angleYaw     = static_cast<float>(mouse.leftMouseDragDeltaX) * rotSpeed * -1.0f;
        const float anglePitch   = static_cast<float>(mouse.leftMouseDragDeltaY) * rotSpeed;
        const float cosHalfYaw   = glm::cos(angleYaw * 0.5f);
        const float sinHalfYaw   = glm::sin(angleYaw * 0.5f);
        const float cosHalfPitch = glm::cos(anglePitch * 0.5f);
        const float sinHalfPitch = glm::sin(anglePitch * 0.5f);

        const glm::vec3 rightVector = cameraTransform->GetRightVector();
        
        const glm::quat rotationYaw   = glm::quat(cosHalfYaw, Transform::WorldUp * sinHalfYaw);
        const glm::quat rotationPitch = glm::quat(cosHalfPitch, -rightVector * sinHalfPitch);
        
        bool applyYaw   = Input::GetMouseDrag(MOUSE_BTN_LEFT);
        bool applyPitch = Input::GetMouseDrag(MOUSE_BTN_LEFT);
        applyPitch     &= glm::abs(glm::dot(Transform::WorldUp, glm::normalize(rotationPitch * cameraTransform->GetPosition()))) < 0.99f;

        if (applyYaw)
        {
            newPosition = rotationYaw * newPosition;
            cameraTransform->Rotate(rotationYaw);
        }
        if (applyPitch)
        {
            newPosition = rotationPitch * newPosition;
            cameraTransform->Rotate(rotationPitch);
        }

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
}

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

    isControllingCamera = false;
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

    if (ImGui::IsWindowHovered() && ImGui::IsWindowFocused() && !ImGuizmo::IsUsing())
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

    if (!isControllingCamera)
    {
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

    Matrix4x4 view = Camera::ActiveCamera->GetViewMatrix();
    Matrix4x4 proj = Camera::ActiveCamera->GetProjectionMatrix();
    Matrix4x4 id   = Matrix4x4(1.0f);
    
    const std::shared_ptr<Hierarchy> hierarchyTab = std::dynamic_pointer_cast<Hierarchy>(editor->FindTabByType(TabType::HIERARCHY));
    const std::shared_ptr<Object> selected = hierarchyTab->SelectedObject.lock();

    ImGuizmo::SetDrawlist();
    ImGuizmo::SetRect(windowPos.x, windowPos.y, windowW, windowH);
    
    if (selected)
    {
        const bool isOrthographic = Camera::ActiveCamera->projectionType == CameraProjection::ORTHOGRAPHIC;
        ImGuizmo::SetOrthographic(isOrthographic);

        Matrix4x4 model = selected->transform->GetModelMatrix();
        Matrix4x4 delta;

        const bool shouldSnap = Input::GetKey(KEYCODE_LEFT_CONTROL);
        bool isModified = ImGuizmo::Manipulate(&view[0][0], &proj[0][0], operation, mode, &model[0][0], &delta[0][0], shouldSnap ? &snapValues[0] : nullptr);

        if (ImGuizmo::IsUsing() && isModified)
        {
            Vector3 position;
            Vector3 rotation;
            Vector3 scale;
            
            Quaternion rotationQuat;

            ImGuizmo::DecomposeMatrixToComponents(&delta[0][0], &position.x, &rotation.x, &scale.x);
            
            switch(operation)
            {
            case ImGuizmo::TRANSLATE:
                selected->transform->Translate(position);
                break;
            case ImGuizmo::ROTATE:
                rotationQuat = Quaternion::FromEuler(rotation * DEG2RAD);
                selected->transform->Rotate(rotationQuat, CoordSpace::WORLD);
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
    const float cubeDimension  = Clamp(windowW * 0.07f, 50.0f, 200.0f);
    const ImVec2 viewGizmoSize = {cubeDimension, cubeDimension};
    const ImVec2 viewGizmoPos  = {windowW - viewGizmoSize.x - offsetX, windowPos.y + 4 * style.FramePadding.y + ImGui::CalcTextSize("Viewport").y + offsetY};
        
    ImGuizmo::ViewManipulate(&view[0][0], &proj[0][0], operation, mode, &id[0][0], 1.0f, viewGizmoPos, viewGizmoSize, IM_COL32(0, 0, 0, 0));

    // DECOMPOSE AND USE THE VIEW MANIPULATION
}

void Viewport::TickViewportCamera()
{
    isControllingCamera = false;
    
    if (Input::GetMouse(MOUSE_BTN_RIGHT))
    {
        CameraFreeMove();
        isControllingCamera = true;
    }
    else if (Input::GetKey(KEYCODE_LEFT_ALT))
    {
        CameraOrbit();
        isControllingCamera = true;
    }
    else
    {
        CameraZoom();
    }
}

void Viewport::CameraFreeMove()
{
    const std::shared_ptr<Transform> cameraTransform = viewportCamera->transform;
    
    const MouseData mouse = Input::Mouse;
    const float rotSpeed  = mouse.sensitivity * Time::DeltaTime * CameraFreeSpeedRot;

    Vector3 offset = Vector3(0.0f, 0.0f, 0.0f);

    const float moveSpeedStep  = mouse.sensitivityScroll * Time::DeltaTime * CameraFreeSpeedMod;
    const float moveSpeedDelta = static_cast<float>(mouse.scrollDeltaYSmooth) * moveSpeedStep;

    if (moveSpeedDelta > FLT_EPSILON || moveSpeedDelta < -FLT_EPSILON)
    {
        CameraFreeSpeedMove = Clamp(CameraFreeSpeedMove + moveSpeedDelta, 2.0f, 30.0f);
    }
        
    if (Input::GetKey(KEYCODE_W))
    {
        offset += cameraTransform->GetForwardVector() * CameraFreeSpeedMove * Time::DeltaTime;
    }
    if (Input::GetKey(KEYCODE_D))
    {
        offset += cameraTransform->GetRightVector() * CameraFreeSpeedMove * Time::DeltaTime;
    }
    if (Input::GetKey(KEYCODE_E))
    {
        offset += cameraTransform->GetUpVector() * CameraFreeSpeedMove * Time::DeltaTime;
    }
    if (Input::GetKey(KEYCODE_S))
    {
        offset -= cameraTransform->GetForwardVector() * CameraFreeSpeedMove * Time::DeltaTime;
    }
    if (Input::GetKey(KEYCODE_A))
    {
        offset -= cameraTransform->GetRightVector() * CameraFreeSpeedMove * Time::DeltaTime;
    }
    if (Input::GetKey(KEYCODE_Q))
    {
        offset -= cameraTransform->GetUpVector() * CameraFreeSpeedMove * Time::DeltaTime;
    }

    cameraFocus += offset;
    cameraTransform->Translate(offset);

    const float angleYaw   = static_cast<float>(mouse.posDeltaX) * rotSpeed * -1.0f;
    const float anglePitch = static_cast<float>(mouse.posDeltaY) * rotSpeed;

    const Vector3 rightVector = cameraTransform->GetRightVector();

    const Quaternion rotationYaw   = Quaternion(Transform::WorldUp, angleYaw);
    const Quaternion rotationPitch = Quaternion(-rightVector, anglePitch);

    bool applyYaw   = Input::GetMouseDrag(MOUSE_BTN_RIGHT);
    bool applyPitch = Input::GetMouseDrag(MOUSE_BTN_RIGHT);
    applyPitch     &= Abs(Vector3::Dot(Transform::WorldUp, (rotationPitch * cameraTransform->GetPosition()).Normalize())) < 0.99f;

    if (applyYaw)
    {
        cameraTransform->Rotate(rotationYaw);
    }
    if (applyPitch)
    {
        cameraTransform->Rotate(rotationPitch);
    }
}

void Viewport::CameraOrbit() const
{
    const std::shared_ptr<Transform> cameraTransform = viewportCamera->transform;
    
    const MouseData mouse = Input::Mouse;

    if (Abs(static_cast<real>(mouse.posDeltaX)) < EPSILON && Abs(static_cast<real>(mouse.posDeltaY)) < EPSILON)
    {
        return;
    }
    
    const float rotSpeed  = mouse.sensitivity * Time::DeltaTime * CameraOrbitSpeed;
    
    const float angleYaw   = static_cast<float>(mouse.posDeltaX) * rotSpeed * -1.0f;
    const float anglePitch = static_cast<float>(mouse.posDeltaY) * rotSpeed;

    const Vector3 rightVector = cameraTransform->GetRightVector();
    
    const Quaternion rotationYaw   = Quaternion(Transform::WorldUp, angleYaw);
    const Quaternion rotationPitch = Quaternion(-rightVector, anglePitch);
    
    bool applyYaw   = Input::GetMouseDrag(MOUSE_BTN_LEFT);
    bool applyPitch = Input::GetMouseDrag(MOUSE_BTN_LEFT);
    applyPitch     &= Abs(Vector3::Dot(Transform::WorldUp, (rotationPitch * cameraTransform->GetPosition()).Normalize())) < 0.99f;

    Vector3 newPosition = cameraTransform->GetPosition();
    
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

    cameraTransform->SetPosition(newPosition);
}

void Viewport::CameraZoom() const
{
    const std::shared_ptr<Transform> cameraTransform = viewportCamera->transform;
    
    const MouseData mouse = Input::Mouse;
    Vector3 newPosition = cameraTransform->GetPosition();
    
    constexpr float minZoomDistance  = 1.0f;
    constexpr float minDistanceSpeed = 5.0f;
    constexpr float maxDistanceSpeed = 150.0f;
    constexpr float minDistanceMult  = 1.0f;
    constexpr float maxDistanceMult  = 5.0f;
    
    const float speedDistMultDelta = Clamp(((cameraTransform->GetPosition()).Length() - minDistanceSpeed) / (maxDistanceSpeed - minDistanceSpeed), 0.0f, 1.0f); 
    const float speedDistanceMult  = speedDistMultDelta * maxDistanceMult + (1.0f - speedDistMultDelta) * minDistanceMult;
        
    const float zoomSpeed       = mouse.sensitivityScroll * Time::DeltaTime * CameraZoomSpeed * speedDistanceMult;
    const float distanceDelta   = static_cast<float>(mouse.scrollDeltaYSmooth) * zoomSpeed * -1.0f;
    const Vector3 zoomDirection = newPosition.Normalize();
    
    const bool applyDistance = (newPosition + zoomDirection * distanceDelta).Length() > minZoomDistance;
    
    newPosition = newPosition + (applyDistance ? zoomDirection * distanceDelta : Vector3());

    cameraTransform->SetPosition(newPosition);
}

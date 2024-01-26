#include "editor_pch.h"
#include "Inspector.h"

#include "Editor.h"
#include "core/components/Transform.h"
#include "core/components/Camera.h"
#include "core/objects/Object.h"
#include "logger/Logger.h"
#include "tabs/Hierarchy.h"
#include "themes/EditorTheme.h"

Inspector::Inspector(Editor* owner) : EditorTab(owner)
{
    title = "Inspector";
}

void Inspector::Tick()
{
    EditorTab::Tick();
    
    if (hierarchyTab == nullptr)
    {
        hierarchyTab = std::dynamic_pointer_cast<Hierarchy>(editor->FindTabByType(TabType::HIERARCHY));
    }

    if (ImGui::Begin(title.c_str(), nullptr, flags))
    {
        if (hierarchyTab != nullptr && !hierarchyTab->SelectedObject.expired())
        {
            DrawComponents();

            ImGui::Spacing();
            ImGui::Separator();
            ImGui::Spacing();

            // Add component button
            const ImVec2 buttonSize = {ImGui::GetContentRegionAvail().x * 0.4f, 30.0f};
            const float buttonPosX = (ImGui::GetContentRegionAvail().x - buttonSize.x) * 0.5f;

            ImGui::SetCursorPosX(buttonPosX);

            if (ImGui::Button("Add Component", buttonSize))
            {
                // Add component logic...
            }
        }
        else if (hierarchyTab == nullptr)
        {
            Logger::Log(LogCategory::WARNING, "The hierarchy tab is not set inside the inspector class...", "Inspector::Tick");
        }

        ImGui::End();
    }
}

void Inspector::DrawComponents()
{
    DrawTransform();

    const std::shared_ptr<Object> selection = hierarchyTab->SelectedObject.lock();
    for (const std::shared_ptr<Component>& component : selection->GetComponents())
    {
        switch (component->GetType())
        {
        case ComponentType::TRANSFORM:
            break;
        case ComponentType::CAMERA:
            DrawCamera(std::dynamic_pointer_cast<Camera>(component));
            break;
        case ComponentType::LIGHT:
            DrawLight();
            break;
        case ComponentType::MESH:
            DrawMesh();
            break;
        default:
            Logger::Log(LogCategory::WARNING, "Unrecognized component type found", "Inspector::DrawComponents");
            break;
        }
    }
}

void Inspector::DrawTransform()
{
    constexpr ImGuiTreeNodeFlags transformFlags = ImGuiTreeNodeFlags_DefaultOpen;

    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, {8.0f, 10.0f});
    ImGui::PushFont(EditorTheme::FontMedium);
    const bool isHeaderOpen = ImGui::CollapsingHeader("Transform", transformFlags);
    ImGui::PopFont();
    ImGui::PopStyleVar();

    if (isHeaderOpen)
    {
        const std::shared_ptr<Object> selection = hierarchyTab->SelectedObject.lock();

        constexpr ImGuiTableFlags tableFlags = ImGuiTableFlags_SizingStretchProp;
        if (ImGui::BeginTable("Transform Data", 2, tableFlags))
        {
            ImGui::TableSetupColumn("##no_label", ImGuiTableColumnFlags_None, componentColFirstWidth);
            ImGui::TableSetupColumn("##no_label", ImGuiTableColumnFlags_None, componentColSecondWidth);

            DrawTransformVector("Position", &selection->transform->position);
            DrawTransformVector("Rotation", &selection->transform->rotation);
            DrawTransformVector("Scale", &selection->transform->scale);

            ImGui::EndTable();
        }
    }
}

void Inspector::DrawTransformVector(const std::string& label, glm::vec3* vector)
{
    ImGui::TableNextRow();

    ImGui::TableNextColumn();

    ImGui::SetCursorPosX(ImGui::GetContentRegionAvail().x * componentLabelIndent);
    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted(label.c_str());

    ImGui::TableNextColumn();

    const float buttonSize = ImGui::CalcTextSize(label.c_str()).y + 2.0f * ImGui::GetStyle().FramePadding.y;
    // Dirty way to calculate button size but works
    const float sizeInputX = ((ImGui::GetContentRegionAvail().x - 2 * ImGui::GetStyle().ItemSpacing.x) * 0.33f) - buttonSize;

    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0);

    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, {0, 0});
    ImGui::PushStyleColor(ImGuiCol_Button, labelColorX);
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, labelColorX);
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, labelColorX);
    ImGui::PushFont(EditorTheme::FontBold);
    ImGui::Button("X", {buttonSize, buttonSize});
    ImGui::PopFont();
    ImGui::PopStyleColor(3);

    ImGui::SameLine();
    ImGui::SetNextItemWidth(sizeInputX);
    ImGui::DragFloat(("##" + label + "1").c_str(), &vector->x, componentDragFloatSpeed, 0, 0, "%.2f");
    ImGui::PopStyleVar();

    ImGui::SameLine();
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, {0, 0});
    ImGui::PushStyleColor(ImGuiCol_Button, labelColorY);
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, labelColorY);
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, labelColorY);
    ImGui::PushFont(EditorTheme::FontBold);
    ImGui::Button("Y");
    ImGui::PopFont();
    ImGui::PopStyleColor(3);

    ImGui::SameLine();
    ImGui::SetNextItemWidth(sizeInputX);
    ImGui::DragFloat(("##" + label + "2").c_str(), &vector->y, componentDragFloatSpeed, 0, 0, "%.2f");
    ImGui::PopStyleVar();

    ImGui::SameLine();
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, {0, 0});
    ImGui::PushStyleColor(ImGuiCol_Button, labelColorZ);
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, labelColorZ);
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, labelColorZ);
    ImGui::PushFont(EditorTheme::FontBold);
    ImGui::Button("Z");
    ImGui::PopFont();
    ImGui::PopStyleColor(3);

    ImGui::SameLine();
    ImGui::SetNextItemWidth(sizeInputX);
    ImGui::DragFloat(("##" + label + "3").c_str(), &vector->z, componentDragFloatSpeed, 0, 0, "%.2f");

    ImGui::PopStyleVar(2);
}

void Inspector::DrawCamera(const std::shared_ptr<Camera>& cameraComponent)
{
    constexpr ImGuiTreeNodeFlags cameraFlags = ImGuiTreeNodeFlags_DefaultOpen;
    
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, {8.0f, 10.0f});
    ImGui::PushFont(EditorTheme::FontMedium);
    const bool isHeaderOpen = ImGui::CollapsingHeader("Camera", cameraFlags);
    ImGui::PopFont();
    ImGui::PopStyleVar();

    if (isHeaderOpen)
    {
        const std::shared_ptr<Object> selection = hierarchyTab->SelectedObject.lock();

        constexpr ImGuiTableFlags tableFlags = ImGuiTableFlags_SizingStretchProp;
        if (ImGui::BeginTable("Camera Data", 2, tableFlags))
        {
            ImGui::TableSetupColumn("##no_label", ImGuiTableColumnFlags_None, componentColFirstWidth);
            ImGui::TableSetupColumn("##no_label", ImGuiTableColumnFlags_None, componentColSecondWidth);

            DrawCameraElements(cameraComponent);

            ImGui::EndTable();
        }
    }
}

void Inspector::DrawCameraElements(const std::shared_ptr<Camera>& cameraComponent)
{
    const char* projectionModes[] = {"Perspective", "Orthographic"};
    const char* selectedItem      = projectionModes[static_cast<int>(cameraComponent->GetProjectionType())];
    float fieldOfView             = cameraComponent->GetPerspectiveFOV();
    float orthoSize               = cameraComponent->GetOrthographicSize();
    float nearZClipping           = cameraComponent->GetNearClipping();
    float farZClipping            = cameraComponent->GetFarClipping();
    
    ImGui::TableNextRow();
    ImGui::TableNextColumn();
    ImGui::SetCursorPosX(ImGui::GetContentRegionAvail().x * componentLabelIndent);
    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted("Projection");
    ImGui::TableNextColumn();
    if (ImGui::BeginCombo("##no_label", selectedItem))
    {
        for (int i = 0; i < 2; i++)
        {
            const bool isSelected = (selectedItem == projectionModes[i]);
            if (ImGui::Selectable(projectionModes[i], isSelected))
            {
                cameraComponent->SetProjectionType(static_cast<CameraProjection>(i));
            }
        }
        
        ImGui::EndCombo();
    }

    if (cameraComponent->GetProjectionType() == CameraProjection::PERSPECTIVE)
    {
        ImGui::TableNextRow();
        ImGui::TableNextColumn();
        ImGui::SetCursorPosX(ImGui::GetContentRegionAvail().x * componentLabelIndent);
        ImGui::AlignTextToFramePadding();
        ImGui::TextUnformatted("Field Of View");
        ImGui::TableNextColumn();
        ImGui::DragFloat("##perspectiveFOV", &fieldOfView, componentDragFloatSpeed, 0, 0, "%.2f");

        cameraComponent->SetPerspectiveFOV(fieldOfView);
    }
    else if (cameraComponent->GetProjectionType() == CameraProjection::ORTHOGRAPHIC)
    {
        ImGui::TableNextRow();
        ImGui::TableNextColumn();
        ImGui::SetCursorPosX(ImGui::GetContentRegionAvail().x * componentLabelIndent);
        ImGui::AlignTextToFramePadding();
        ImGui::TextUnformatted("View Size");
        ImGui::TableNextColumn();
        ImGui::DragFloat("##orthoSize", &orthoSize, componentDragFloatSpeed, 0, 0, "%.2f");

        cameraComponent->SetOrthographicSize(orthoSize);
    }

    ImGui::TableNextRow();
    ImGui::TableNextColumn();
    ImGui::SetCursorPosX(ImGui::GetContentRegionAvail().x * componentLabelIndent);
    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted("Near Z");
    ImGui::TableNextColumn();
    ImGui::DragFloat("##nearZPlane", &nearZClipping, componentDragFloatSpeed, 0, 0, "%.2f");

    ImGui::TableNextRow();
    ImGui::TableNextColumn();
    ImGui::SetCursorPosX(ImGui::GetContentRegionAvail().x * componentLabelIndent);
    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted("Far Z");
    ImGui::TableNextColumn();
    ImGui::DragFloat("##farZPlane", &farZClipping, componentDragFloatSpeed, 0, 0, "%.2f");
    
    cameraComponent->SetNearClipping(nearZClipping);
    cameraComponent->SetFarClipping(farZClipping);
}

void Inspector::DrawLight()
{
}

void Inspector::DrawMesh()
{
}

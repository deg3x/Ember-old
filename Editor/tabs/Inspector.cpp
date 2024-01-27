#include "editor_pch.h"
#include "Inspector.h"

#include "Editor.h"
#include "core/components/Transform.h"
#include "core/components/Camera.h"
#include "core/components/Light.h"
#include "core/components/meshes/Mesh.h"
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

            DrawAddComponentButton();
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
            DrawLight(std::dynamic_pointer_cast<Light>(component));
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
            ImGui::TableSetupColumn("##no_label", ImGuiTableColumnFlags_None, compColWidthFirstTransf);
            ImGui::TableSetupColumn("##no_label", ImGuiTableColumnFlags_None, compColWidthSecondTransf);

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

    ImGui::SetCursorPosX(ImGui::GetContentRegionAvail().x * compLabelIndent);
    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted(label.c_str());

    ImGui::TableNextColumn();

    // Dirty way to calculate button size but works
    const float buttonSize = ImGui::CalcTextSize(label.c_str()).y + 2.0f * ImGui::GetStyle().FramePadding.y;
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
    ImGui::DragFloat(("##" + label + "1").c_str(), &vector->x, compDragFloatSpeed, 0, 0, "%.2f");
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
    ImGui::DragFloat(("##" + label + "2").c_str(), &vector->y, compDragFloatSpeed, 0, 0, "%.2f");
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
    ImGui::DragFloat(("##" + label + "3").c_str(), &vector->z, compDragFloatSpeed, 0, 0, "%.2f");

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
            ImGui::TableSetupColumn("##no_label", ImGuiTableColumnFlags_None, compColWidthFirst);
            ImGui::TableSetupColumn("##no_label", ImGuiTableColumnFlags_None, compColWidthSecond);

            DrawCameraElements(cameraComponent);

            ImGui::EndTable();
        }
    }
}

void Inspector::DrawCameraElements(const std::shared_ptr<Camera>& cameraComponent)
{
    const char* projectionModes[] = {"Perspective", "Orthographic"};
    const char* selectedItem      = projectionModes[static_cast<int>(cameraComponent->projectionType)];
    
    ImGui::TableNextRow();
    ImGui::TableNextColumn();
    ImGui::SetCursorPosX(ImGui::GetContentRegionAvail().x * compLabelIndent);
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
                cameraComponent->projectionType = static_cast<CameraProjection>(i);
            }
        }
        
        ImGui::EndCombo();
    }

    if (cameraComponent->projectionType == CameraProjection::PERSPECTIVE)
    {
        DrawRowLabelDragFloat("Field Of View", cameraComponent->perspectiveFOV);
    }
    else if (cameraComponent->projectionType == CameraProjection::ORTHOGRAPHIC)
    {
        DrawRowLabelDragFloat("View Size", cameraComponent->orthographicSize);
    }
    
    DrawRowLabelDragFloat("Near Z", cameraComponent->nearClipping);
    DrawRowLabelDragFloat("Far Z", cameraComponent->farClipping);
}

void Inspector::DrawLight(const std::shared_ptr<Light>& lightComponent)
{
    constexpr ImGuiTreeNodeFlags lightFlags = ImGuiTreeNodeFlags_DefaultOpen;
    
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, {8.0f, 10.0f});
    ImGui::PushFont(EditorTheme::FontMedium);
    const bool isHeaderOpen = ImGui::CollapsingHeader("Light", lightFlags);
    ImGui::PopFont();
    ImGui::PopStyleVar();

    if (isHeaderOpen)
    {
        const std::shared_ptr<Object> selection = hierarchyTab->SelectedObject.lock();

        constexpr ImGuiTableFlags tableFlags = ImGuiTableFlags_SizingStretchProp;
        if (ImGui::BeginTable("Light Data", 2, tableFlags))
        {
            ImGui::TableSetupColumn("##no_label", ImGuiTableColumnFlags_None, compColWidthFirst);
            ImGui::TableSetupColumn("##no_label", ImGuiTableColumnFlags_None, compColWidthSecond);

            DrawLightElements(lightComponent);

            ImGui::EndTable();
        }
    }
}

void Inspector::DrawLightElements(const std::shared_ptr<Light>& lightComponent)
{
    const char* lightTypes[] = {"Directional Light", "Point Light", "Spotlight"};
    const char* selectedItem = lightTypes[static_cast<int>(lightComponent->lightType)];

    ImGui::TableNextRow();
    ImGui::TableNextColumn();
    ImGui::SetCursorPosX(ImGui::GetContentRegionAvail().x * compLabelIndent);
    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted("Light Type");
    ImGui::TableNextColumn();
    if (ImGui::BeginCombo("##no_label", selectedItem))
    {
        for (int i = 0; i < 3; i++)
        {
            const bool isSelected = (selectedItem == lightTypes[i]);
            if (ImGui::Selectable(lightTypes[i], isSelected))
            {
                lightComponent->lightType = static_cast<LightType>(i);
            }
        }
        
        ImGui::EndCombo();
    }

    float* diffuse = glm::value_ptr(lightComponent->diffuse);
    float* ambient = glm::value_ptr(lightComponent->ambient);
    
    DrawRowLabelColor3("Diffuse", diffuse);
    DrawRowLabelColor3("Ambient", ambient);
    DrawRowLabelDragFloat("Intensity", lightComponent->intensity);

    switch (lightComponent->lightType)
    {
    case LightType::DIRECTIONAL:
        break;
    case LightType::POINT:
        DrawRowLabelDragFloat("Constant", lightComponent->pointLightData.constantAttenuation);
        DrawRowLabelDragFloat("Linear", lightComponent->pointLightData.linearAttenuation);
        DrawRowLabelDragFloat("Quadratic", lightComponent->pointLightData.quadraticAttenuation);
        break;
    case LightType::SPOTLIGHT:
        DrawRowLabelDragFloat("Constant", lightComponent->spotlightData.constantAttenuation);
        DrawRowLabelDragFloat("Linear", lightComponent->spotlightData.linearAttenuation);
        DrawRowLabelDragFloat("Quadratic", lightComponent->spotlightData.quadraticAttenuation);
        DrawRowLabelDragFloat("Cutoff Angle", lightComponent->spotlightData.cutOffAngle);
        DrawRowLabelDragFloat("Cutoff Angle (out)", lightComponent->spotlightData.cutOffAngleOut);
        break;
    default:
        Logger::Log(LogCategory::WARNING, "Unknown light type. Skipping...", "Inspector::DrawLightComponents");
        break;
    }
}

void Inspector::DrawMesh()
{
    constexpr ImGuiTreeNodeFlags meshFlags = ImGuiTreeNodeFlags_DefaultOpen;
    
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, {8.0f, 10.0f});
    ImGui::PushFont(EditorTheme::FontMedium);
    const bool isHeaderOpen = ImGui::CollapsingHeader("Mesh", meshFlags);
    ImGui::PopFont();
    ImGui::PopStyleVar();
}

void Inspector::DrawAddComponentButton()
{
    const ImVec2 buttonSize = {ImGui::GetContentRegionAvail().x * 0.5f, 30.0f};
    const float buttonPosX = (ImGui::GetContentRegionAvail().x - buttonSize.x) * 0.5f;

    ImGui::SetCursorPosX(buttonPosX);

    if (ImGui::Button("Add Component", buttonSize))
    {
        ImGui::OpenPopup("AddComponentPopup", ImGuiPopupFlags_None);
    }

    const ImVec2 buttonRectMin = ImGui::GetItemRectMin();
    const ImVec2 buttonRectMax = ImGui::GetItemRectMax();
    ImGui::SetNextWindowSize({buttonSize.x, 0.0f});
    ImGui::SetNextWindowPos({buttonRectMin.x, buttonRectMax.y});
    constexpr ImGuiWindowFlags popupFlags = ImGuiWindowFlags_None;
    if (ImGui::BeginPopup("AddComponentPopup", popupFlags))
    {
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, {0.0f, 3.0f});
        ImGui::PushStyleColor(ImGuiCol_Button, EditorTheme::ColorGreen);
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, EditorTheme::ColorGreenActive);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, EditorTheme::ColorGreenHovered);

        const ImVec2 entriesSize = {ImGui::GetContentRegionAvail().x, 0.0f};

        if (ImGui::Button("Camera", entriesSize))
        {
            hierarchyTab->SelectedObject.lock()->CreateComponent<Camera>();
        }
        if (ImGui::Button("Light", entriesSize))
        {
            hierarchyTab->SelectedObject.lock()->CreateComponent<Light>();
        }
        if (ImGui::Button("Mesh", entriesSize))
        {
            // Not working, fix
            // hierarchyTab->SelectedObject.lock()->CreateComponent<Mesh>();
        }

        ImGui::PopStyleColor(3);
        ImGui::PopStyleVar(2);
        
        ImGui::EndPopup();
    }
}

void Inspector::DrawRowLabelDragFloat(const std::string& label, float& target)
{
    ImGui::TableNextRow();
    ImGui::TableNextColumn();
    ImGui::SetCursorPosX(ImGui::GetContentRegionAvail().x * compLabelIndent);
    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted(label.c_str());

    ImGui::TableNextColumn();
    ImGui::DragFloat(("##" + label).c_str(), &target, compDragFloatSpeed, 0, 0, "%.3f");
}

void Inspector::DrawRowLabelColor3(const std::string& label, float target[3])
{
    ImGui::TableNextRow();
    ImGui::TableNextColumn();
    ImGui::SetCursorPosX(ImGui::GetContentRegionAvail().x * compLabelIndent);
    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted(label.c_str());
    
    ImGui::TableNextColumn();
    const ImVec4 buttonColor = {target[0], target[1], target[2], 1.0f};
    const ImVec2 buttonSize  = {ImGui::GetContentRegionAvail().x * 0.65f, 0.0f};
    if (ImGui::ColorButton(label.c_str(), buttonColor, ImGuiColorEditFlags_Float, buttonSize))
    {
        ImGui::OpenPopup(("ColorPicker" + label).c_str(), ImGuiPopupFlags_None);
    }

    constexpr ImGuiWindowFlags popupFlags = ImGuiWindowFlags_None;
    if (ImGui::BeginPopup(("ColorPicker" + label).c_str(), popupFlags))
    {
        ImGui::ColorPicker3(("##" + label).c_str(), target);

        if (ImGui::Button("Close"))
            ImGui::CloseCurrentPopup();
        
        ImGui::EndPopup();
    }
}

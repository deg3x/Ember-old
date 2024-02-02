#include "editor_pch.h"
#include "Inspector.h"

#include "Editor.h"
#include "core/Material.h"
#include "core/components/Transform.h"
#include "core/components/Camera.h"
#include "core/components/Light.h"
#include "core/components/Mesh.h"
#include "core/Object.h"
#include "core/Shader.h"
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
            DrawHeader();
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

void Inspector::DrawHeader()
{
    const std::shared_ptr<Object> selection = hierarchyTab->SelectedObject.lock();
    const ImGuiStyle& style = ImGui::GetStyle();

    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, {8.0f, 0.0f});
    ImGui::PushFont(EditorTheme::FontMedium);

    const ImVec4 color = {EditorTheme::ColorGreen.x, EditorTheme::ColorGreen.y, EditorTheme::ColorGreen.z, EditorTheme::ColorGreen.w};
    const ImU32 headerColor   = IM_COL32(color.x * 255, color.y * 255, color.z * 255, color.w * 255);
    const ImVec2 headerSize   = {ImGui::GetContentRegionAvail().x, 50.0f};
    const ImVec2 headerPosMin = ImGui::GetCursorScreenPos();
    const ImVec2 headerPosMax = {headerPosMin.x + headerSize.x, headerPosMin.y + headerSize.y};
    
    ImGui::GetWindowDrawList()->AddRectFilled(headerPosMin, headerPosMax, headerColor, style.FrameRounding);

    const ImVec2 checkboxPosition = {headerPosMin.x + 20.0f, headerPosMin.y + (headerSize.y - style.FramePadding.y - ImGui::GetFontSize()) * 0.5f};
    ImGui::SetCursorScreenPos(checkboxPosition);

    ImGui::Checkbox("##no_label", &selection->isActive);

    ImGui::SameLine();

    const float textFieldPosY = headerPosMin.y + (headerSize.y - style.FramePadding.y - ImGui::GetFontSize()) * 0.5f;
    const ImVec2 textFieldPos = {ImGui::GetItemRectMax().x + 20.0f, textFieldPosY};
    ImGui::SetCursorScreenPos(textFieldPos);

    ImGui::TextUnformatted(selection->name.c_str());
    
    ImGui::PopStyleVar();
    ImGui::PopFont();
    
    ImGui::SetCursorScreenPos({headerPosMin.x, headerPosMax.y + style.ItemSpacing.y});
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
            {
                std::shared_ptr<Mesh> meshComponent = std::dynamic_pointer_cast<Mesh>(component);
                DrawMesh(meshComponent);
                DrawMaterial(meshComponent->material);
                break;
            }
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
    
    ImVec4 popupBgColor = ImGui::GetStyle().Colors[ImGuiCol_PopupBg];
    popupBgColor.w = 0.97f;
    ImGui::PushStyleColor(ImGuiCol_PopupBg, popupBgColor);
    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x * compColWidthSecondMult);
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
    ImGui::PopStyleColor(1);

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

    ImVec4 popupBgColor = ImGui::GetStyle().Colors[ImGuiCol_PopupBg];
    popupBgColor.w = 0.97f;
    ImGui::PushStyleColor(ImGuiCol_PopupBg, popupBgColor);
    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x * compColWidthSecondMult);
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
    ImGui::PopStyleColor(1);

    float* color   = glm::value_ptr(lightComponent->color);
    float* ambient = glm::value_ptr(lightComponent->ambient);
    
    DrawRowLabelColor3("Color", color);
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
        DrawRowLabelDragFloat("Constant", lightComponent->spotLightData.constantAttenuation);
        DrawRowLabelDragFloat("Linear", lightComponent->spotLightData.linearAttenuation);
        DrawRowLabelDragFloat("Quadratic", lightComponent->spotLightData.quadraticAttenuation);
        DrawRowLabelDragFloat("Cutoff Angle", lightComponent->spotLightData.cutOffAngle);
        DrawRowLabelDragFloat("Cutoff Angle (out)", lightComponent->spotLightData.cutOffAngleOut);
        break;
    default:
        Logger::Log(LogCategory::WARNING, "Unknown light type. Skipping...", "Inspector::DrawLightComponents");
        break;
    }
}

void Inspector::DrawMesh(const std::shared_ptr<Mesh>& meshComponent)
{
    constexpr ImGuiTreeNodeFlags meshFlags = ImGuiTreeNodeFlags_DefaultOpen;
    
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, {8.0f, 10.0f});
    ImGui::PushFont(EditorTheme::FontMedium);
    const bool isHeaderOpen = ImGui::CollapsingHeader("Mesh", meshFlags);
    ImGui::PopFont();
    ImGui::PopStyleVar();

    if (isHeaderOpen)
    {
        constexpr ImGuiTableFlags tableFlags = ImGuiTableFlags_SizingStretchProp;
        if (ImGui::BeginTable("Mesh Data", 2, tableFlags))
        {
            ImGui::TableSetupColumn("##no_label", ImGuiTableColumnFlags_None, compColWidthFirst);
            ImGui::TableSetupColumn("##no_label", ImGuiTableColumnFlags_None, compColWidthSecond);

            DrawMeshElements(meshComponent);

            ImGui::EndTable();
        }
    }
}

void Inspector::DrawMeshElements(const std::shared_ptr<Mesh>& meshComponent)
{
    std::string renderQueue;
    switch (meshComponent->meshType)
    {
    case MeshType::OPAQUE:
        renderQueue = "Opaque";
        break;
    case MeshType::TRANSPARENT:
        renderQueue = "Transparent";
        break;
    }

    DrawRowLabelText("Render queue", renderQueue);
    DrawRowLabelText("Vertices", std::to_string(meshComponent->GetVertexData().size()));
    DrawRowLabelText("Indices", std::to_string(meshComponent->GetIndices().size()));
}

void Inspector::DrawMaterial(const std::shared_ptr<Material>& material)
{
    constexpr ImGuiTreeNodeFlags materialFlags = ImGuiTreeNodeFlags_DefaultOpen;
    
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, {8.0f, 10.0f});
    ImGui::PushFont(EditorTheme::FontMedium);
    const bool isHeaderOpen = ImGui::CollapsingHeader("Material", materialFlags);
    ImGui::PopFont();
    ImGui::PopStyleVar();

    if (isHeaderOpen)
    {
        constexpr ImGuiTableFlags tableFlags = ImGuiTableFlags_SizingStretchProp;
        if (ImGui::BeginTable("Material Data", 2, tableFlags))
        {
            ImGui::TableSetupColumn("##no_label", ImGuiTableColumnFlags_None, compColWidthFirst);
            ImGui::TableSetupColumn("##no_label", ImGuiTableColumnFlags_None, compColWidthSecond);

            DrawMaterialElements(material);

            ImGui::EndTable();
        }
    }
}

void Inspector::DrawMaterialElements(const std::shared_ptr<Material>& material)
{
    std::unordered_set<MaterialProperty, MaterialProperty> properties = material->GetProperties();

    std::string vertPath = material->GetShader()->GetVertexPath();
    std::string fragPath = material->GetShader()->GetFragmentPath();

    vertPath = vertPath.substr(vertPath.find_last_of("/\\") + 1);
    fragPath = fragPath.substr(fragPath.find_last_of("/\\") + 1);
    
    DrawRowLabelText("Vertex Shader", vertPath);
    DrawRowLabelText("Fragment Shader", fragPath);

    for (auto property : properties)
    {
        switch(property.type)
        {
        case MaterialProperty::PropertyType::INTEGER:
            DrawRowLabelDragInt(property.name, property.value.intVal);
            material->SetProperty(property.name, property.value.intVal);
            break;
        case MaterialProperty::PropertyType::BOOLEAN:
            Logger::Log(LogCategory::WARNING, "Material property with no proper Draw function found: " + property.name, "Inspector::DrawMaterialElements");
            break;
        case MaterialProperty::PropertyType::FLOAT:
            DrawRowLabelDragFloat(property.name, property.value.floatVal);
            material->SetProperty(property.name, property.value.floatVal);
            break;
        case MaterialProperty::PropertyType::VECTOR3:
            DrawRowLabelDragFloat3(property.name, property.value.vector3Val);
            material->SetProperty(property.name, property.value.vector3Val);
            break;
        case MaterialProperty::PropertyType::VECTOR4:
            DrawRowLabelDragFloat4(property.name, property.value.vector4Val);
            material->SetProperty(property.name, property.value.vector4Val);
            break;
        case MaterialProperty::PropertyType::MATRIX4X4:
            Logger::Log(LogCategory::WARNING, "Material property with no proper Draw function found: " + property.name, "Inspector::DrawMaterialElements");
            break;
        }
    }
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

        const ImVec2 entriesSize = {ImGui::GetContentRegionAvail().x, 0.0f};

        if (ImGui::Button("Camera", entriesSize))
        {
            hierarchyTab->SelectedObject.lock()->CreateComponent<Camera>();
        }
        if (ImGui::Button("Light", entriesSize))
        {
            hierarchyTab->SelectedObject.lock()->CreateComponent<Light>();
        }
        /*if (ImGui::Button("Mesh", entriesSize))
        {
            // Not working, fix
            hierarchyTab->SelectedObject.lock()->CreateComponent<Mesh>();
        }*/

        ImGui::PopStyleVar(2);
        
        ImGui::EndPopup();
    }
}

void Inspector::DrawRowLabelDragInt(const std::string& label, int& target)
{
    ImGui::TableNextRow();
    ImGui::TableNextColumn();
    ImGui::SetCursorPosX(ImGui::GetContentRegionAvail().x * compLabelIndent);
    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted(label.c_str());

    ImGui::TableNextColumn();
    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x * compColWidthSecondMult);
    ImGui::DragInt(("##" + label).c_str(), &target, compDragFloatSpeed);
}

void Inspector::DrawRowLabelDragFloat(const std::string& label, float& target)
{
    ImGui::TableNextRow();
    ImGui::TableNextColumn();
    ImGui::SetCursorPosX(ImGui::GetContentRegionAvail().x * compLabelIndent);
    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted(label.c_str());

    ImGui::TableNextColumn();
    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x * compColWidthSecondMult);
    ImGui::DragFloat(("##" + label).c_str(), &target, compDragFloatSpeed, 0, 0, "%.3f");
}

void Inspector::DrawRowLabelDragFloat3(const std::string& label, glm::vec3& target)
{
    ImGui::TableNextRow();
    ImGui::TableNextColumn();
    ImGui::SetCursorPosX(ImGui::GetContentRegionAvail().x * compLabelIndent);
    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted(label.c_str());

    ImGui::TableNextColumn();

    float intermediate[3] = { target.x, target.y, target.z };

    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x * compColWidthSecondMult);
    ImGui::DragFloat3(("##" + label).c_str(), intermediate, compDragFloatSpeed, 0, 0, "%.3f");

    target = { intermediate[0], intermediate[1], intermediate[2] };
}

void Inspector::DrawRowLabelDragFloat4(const std::string& label, glm::vec4& target)
{
    ImGui::TableNextRow();
    ImGui::TableNextColumn();
    ImGui::SetCursorPosX(ImGui::GetContentRegionAvail().x * compLabelIndent);
    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted(label.c_str());

    ImGui::TableNextColumn();

    float intermediate[4] = { target.x, target.y, target.z, target.w };

    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x * compColWidthSecondMult);
    ImGui::DragFloat4(("##" + label).c_str(), intermediate, compDragFloatSpeed, 0, 0, "%.3f");

    target = { intermediate[0], intermediate[1], intermediate[2], intermediate[3] };
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
    const ImVec2 buttonSize  = {ImGui::GetContentRegionAvail().x * compColWidthSecondMult, 0.0f};
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

void Inspector::DrawRowLabelText(const std::string& label, const std::string& text)
{
    ImGui::TableNextRow();
    ImGui::TableNextColumn();
    ImGui::SetCursorPosX(ImGui::GetContentRegionAvail().x * compLabelIndent);
    ImGui::TextUnformatted(label.c_str());

    ImGui::TableNextColumn();
    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x * compColWidthSecondMult);
    ImGui::TextUnformatted(text.c_str());
}

#include "Inspector.h"

#include <glm/glm.hpp>

#include "Editor.h"
#include "core/components/Transform.h"
#include "core/objects/Object.h"
#include "logger/Logger.h"
#include "tabs/Hierarchy.h"

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
        if (hierarchyTab != nullptr)
        {
            DrawTransform();
        }
        else
        {
            Logger::Log(LogCategory::WARNING, "The hierarchy tab is not set inside the inspector class...", "Inspector::Tick");
        }
        
        ImGui::End();
    }
}

void Inspector::DrawTransform()
{
    if (hierarchyTab->SelectedObject.expired())
    {
        return;
    }
    
    constexpr ImGuiTreeNodeFlags transformFlags = ImGuiTreeNodeFlags_DefaultOpen;
    if (ImGui::CollapsingHeader("Transform", transformFlags))
    {
        const std::shared_ptr<Object> selection = hierarchyTab->SelectedObject.lock();

        constexpr ImGuiTableFlags tableFlags = ImGuiTableFlags_SizingStretchProp;
        if (ImGui::BeginTable("Transform Data", 2, tableFlags))
        {
            ImGui::TableSetupColumn("##no_label", ImGuiTableColumnFlags_None, 0.2f);
            ImGui::TableSetupColumn("##no_label", ImGuiTableColumnFlags_None, 0.8f);
            
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
    ImGui::TextUnformatted(label.c_str());

    ImGui::TableNextColumn();
    const float sizeX = (ImGui::GetContentRegionAvail().x - 2 * ImGui::GetStyle().ItemSpacing.x) * 0.33f;
    ImGui::SetNextItemWidth(sizeX);
    ImGui::DragFloat(("##" + label + "1").c_str(), &vector->x, 0.1f);
    ImGui::SameLine();
    ImGui::SetNextItemWidth(sizeX);
    ImGui::DragFloat(("##" + label + "2").c_str(), &vector->y, 0.1f);
    ImGui::SameLine();
    ImGui::SetNextItemWidth(sizeX);
    ImGui::DragFloat(("##" + label + "3").c_str(), &vector->z, 0.1f);
}

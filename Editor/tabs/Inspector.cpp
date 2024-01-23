#include "Inspector.h"

#include <glm/glm.hpp>

#include "Editor.h"
#include "core/components/Transform.h"
#include "core/objects/Object.h"
#include "imgui/imgui_internal.h"
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
        if (hierarchyTab != nullptr && !hierarchyTab->SelectedObject.expired())
        {
            DrawTransform();

            // Add component button
            const ImVec2 buttonSize = {ImGui::GetContentRegionAvail().x * 0.4f, 30.0f};
            const float buttonPosX  = (ImGui::GetContentRegionAvail().x - buttonSize.x) * 0.5f;

            ImGui::SetCursorPosX(buttonPosX);
            
            if (ImGui::Button("Add Component", buttonSize))
            {
                // Add component logic...
            }
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
    constexpr ImGuiTreeNodeFlags transformFlags = ImGuiTreeNodeFlags_DefaultOpen;
    
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, {8.0f, 10.0f});
    ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0);
    const bool isHeaderOpen = ImGui::CollapsingHeader("Transform", transformFlags);
    ImGui::PopStyleVar(2);

    if (isHeaderOpen)
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
    
    const float labelStartX = ImGui::GetContentRegionAvail().x * 0.3f;
    ImGui::SetCursorPosX(labelStartX);
    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted(label.c_str());

    ImGui::TableNextColumn();
    
    const float buttonSize  = ImGui::CalcTextSize(label.c_str()).y + 2.0f * ImGui::GetStyle().FramePadding.y; // Dirty way to calculate button size but works
    const float sizeInputX  = ((ImGui::GetContentRegionAvail().x - 2 * ImGui::GetStyle().ItemSpacing.x) * 0.33f) - buttonSize;

    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0);
    ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0);

    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, {0, 0});
    ImGui::PushStyleColor(ImGuiCol_Button, labelColorX);
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, labelColorX);
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, labelColorX);
    ImGui::Button("X", {buttonSize, buttonSize});
    ImGui::PopStyleColor(3);
    
    ImGui::SameLine();
    ImGui::SetNextItemWidth(sizeInputX);
    ImGui::DragFloat(("##" + label + "1").c_str(), &vector->x, 0.1f, 0, 0, "%.2f");
    ImGui::PopStyleVar();
    
    ImGui::SameLine();
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, {0, 0});
    ImGui::PushStyleColor(ImGuiCol_Button, labelColorY);
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, labelColorY);
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, labelColorY);
    ImGui::Button("Y");
    ImGui::PopStyleColor(3);
    
    ImGui::SameLine();
    ImGui::SetNextItemWidth(sizeInputX);
    ImGui::DragFloat(("##" + label + "2").c_str(), &vector->y, 0.1f, 0, 0, "%.2f");
    ImGui::PopStyleVar();
    
    ImGui::SameLine();
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, {0, 0});
    ImGui::PushStyleColor(ImGuiCol_Button, labelColorZ);
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, labelColorZ);
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, labelColorZ);
    ImGui::Button("Z");
    ImGui::PopStyleColor(3);
    
    ImGui::SameLine();
    ImGui::SetNextItemWidth(sizeInputX);
    ImGui::DragFloat(("##" + label + "3").c_str(), &vector->z, 0.1f, 0, 0, "%.2f");

    ImGui::PopStyleVar(3);
}
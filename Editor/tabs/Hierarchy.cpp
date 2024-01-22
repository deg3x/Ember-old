#include "Hierarchy.h"

#include "core/World.h"
#include "core/objects/Object.h"
#include "imgui/imgui_internal.h"
#include "input/Input.h"
#include "logger/Logger.h"

Hierarchy::Hierarchy(Editor* owner) : EditorTab(owner)
{
    title = "Hierarchy";

    selectedObject.reset();
}

void Hierarchy::Tick()
{
    EditorTab::Tick();

    std::string entityStr = selectedObject.expired() ? "nullptr" : selectedObject.lock()->name;

    if (!selectedObject.expired())
        Logger::Log("Selected entity: " + entityStr);

    std::vector<std::shared_ptr<Object>> objects;
    objects.reserve(World::objQueueOpaque.size() + World::objQueueTransparent.size());
    objects.insert(objects.end(), World::objQueueOpaque.begin(), World::objQueueOpaque.end());
    objects.insert(objects.end(), World::objQueueTransparent.begin(), World::objQueueTransparent.end());

    if (ImGui::Begin(title.c_str(), nullptr, flags))
    {
        ImGuiTreeNodeFlags treeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_SpanFullWidth;
        if (ImGui::TreeNodeEx("World", treeFlags))
        {
            treeFlags = ImGuiTreeNodeFlags_Leaf;
            
            bool anyHovered = false;
            
            for (const std::shared_ptr<Object>& object : objects)
            {
                if (!hoveredObject.expired())
                {
                    if (hoveredObject.lock() == object)
                    {
                        treeFlags |= ImGuiTreeNodeFlags_Selected;
                    }
                    else
                    {
                        treeFlags &= ~ImGuiTreeNodeFlags_Selected;
                    }
                }
                if(ImGui::TreeNodeEx(object->name.c_str(), treeFlags))
                {
                    if(ImGui::IsItemHovered())
                    {
                        hoveredObject = object;
                        anyHovered    = true;
                    }
                    
                    ImGui::TreePop();
                }
            }

            if (!anyHovered)
            {
                hoveredObject.reset();
            }
            
            ImGui::TreePop();
        }
        
        ImGui::End();
    }

    if (!ImGui::IsWindowHovered())
    {
        return;
    }

    if (Input::GetMouse(0))
    {
        Logger::Log("MOUSE PRESSED LAST FRAME!!!");
    }

    if (Input::Mouse.leftButtonPressed)
    {
        selectedObject = hoveredObject;
    }
    else if (Input::Mouse.leftButtonPressedLastFrame)
    {
        if (hoveredObject.expired())    // Clicked in empty space
        {
            selectedObject.reset();
        }
        else if (selectedObject.lock() == hoveredObject.lock())
        {
            // Submit to inspector
        }
        else    // This is a drag case. We do not handle it for now
        {
            selectedObject.reset();
        }
    }
}

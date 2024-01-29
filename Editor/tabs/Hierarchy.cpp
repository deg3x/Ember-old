#include "editor_pch.h"
#include "Hierarchy.h"

#include "core/World.h"
#include "core/Object.h"
#include "core/components/Mesh.h"
#include "core/materials/MaterialBlinnPhong.h"
#include "input/Input.h"
#include "themes/EditorTheme.h"
#include "utils/procedural/Cube.h"
#include "utils/procedural/Plane.h"
#include "utils/procedural/Sphere.h"

Hierarchy::Hierarchy(Editor* owner) : EditorTab(owner)
{
    title = "Hierarchy";

    SelectedObject.reset();
    hoveredObject.reset();
    clickedObject.reset();
}

void Hierarchy::Tick()
{
    EditorTab::Tick();

    std::vector<std::shared_ptr<Object>> objects;
    objects.reserve(World::objQueueOpaque.size() + World::objQueueTransparent.size());
    objects.insert(objects.end(), World::objQueueOpaque.begin(), World::objQueueOpaque.end());
    objects.insert(objects.end(), World::objQueueTransparent.begin(), World::objQueueTransparent.end());
    
    if (ImGui::Begin(title.c_str(), nullptr, flags))
    {
        ImGuiTreeNodeFlags treeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_SpanFullWidth | ImGuiTreeNodeFlags_FramePadding;

        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, {0.0f, 0.0f});
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, {6.0f, 5.0f});
        ImGui::PushStyleColor(ImGuiCol_Header, EditorTheme::ColorGreen);
        ImGui::PushStyleColor(ImGuiCol_HeaderHovered, EditorTheme::ColorGreenHovered);
        ImGui::PushStyleColor(ImGuiCol_HeaderActive, EditorTheme::ColorGreenActive);
        if (ImGui::TreeNodeEx("World", treeFlags))
        {
            treeFlags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_SpanFullWidth | ImGuiTreeNodeFlags_FramePadding;
            
            bool anyHovered = false;
            
            for (const std::shared_ptr<Object>& object : objects)
            {
                if (!SelectedObject.expired())
                {
                    if (SelectedObject.lock() == object)
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

        ImGui::PopStyleColor(3);
        ImGui::PopStyleVar(2);

        DrawPopupAddObject();
        
        HandleMouseBehavior();
        
        ImGui::End();
    }
}

void Hierarchy::HandleMouseBehavior()
{
    if (!ImGui::IsWindowHovered())
    {
        return;
    }

    if (Input::Mouse.leftButtonPressed)
    {
        clickedObject = hoveredObject;
    }
    else if (Input::Mouse.leftButtonPressedLastFrame)
    {
        if (hoveredObject.expired())    // Clicked in empty space
        {
            SelectedObject.reset();
            clickedObject.reset();
        }
        else if (clickedObject.lock() == hoveredObject.lock())
        {
            SelectedObject = clickedObject;
        }
        else    // This is a mouse drag case. We do not handle it for now
        {
            SelectedObject.reset();
            clickedObject.reset();
        }
    }

    const bool isRightClick = Input::Mouse.rightButtonPressedLastFrame && !Input::Mouse.rightButtonPressed;
    if (isRightClick)
    {
        ImGui::OpenPopup("AddObjectPopup", ImGuiPopupFlags_None);
    }
}

void Hierarchy::DrawPopupAddObject()
{
    ImGui::PushStyleVar(ImGuiStyleVar_PopupRounding, 1.0f);
    
    constexpr ImGuiWindowFlags popupFlags = ImGuiWindowFlags_None;
    if (ImGui::BeginPopup("AddObjectPopup", popupFlags))
    {
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, {0.0f, 3.0f});

        ImGui::TextUnformatted("Create New Object...");

        ImGui::Separator();

        const ImVec2 entriesSize = {ImGui::GetContentRegionAvail().x, 0.0f};
        if (ImGui::Button("Cube", entriesSize))
        {
            const std::shared_ptr<Object> cubeObject = std::make_shared<Object>("Cube");
            const std::shared_ptr<Mesh> cubeMesh = cubeObject->CreateComponent<Mesh>();
            const std::shared_ptr<MaterialBlinnPhong> mat = std::make_shared<MaterialBlinnPhong>();
            cubeMesh->material = mat;
            Cube::GenerateCube(cubeMesh);
            World::AddObject(cubeObject);
        }
        if (ImGui::Button("Sphere", entriesSize))
        {
            const std::shared_ptr<Object> sphereObject = std::make_shared<Object>("Sphere");
            const std::shared_ptr<Mesh> sphereMesh = sphereObject->CreateComponent<Mesh>();
            const std::shared_ptr<MaterialBlinnPhong> mat = std::make_shared<MaterialBlinnPhong>();
            sphereMesh->material = mat;
            Sphere::GenerateSphere(32, 32, 0.5f, sphereMesh);
            World::AddObject(sphereObject);
        }
        if (ImGui::Button("Plane", entriesSize))
        {
            const std::shared_ptr<Object> planeObject = std::make_shared<Object>("Plane");
            const std::shared_ptr<Mesh> planeMesh = planeObject->CreateComponent<Mesh>();
            const std::shared_ptr<MaterialBlinnPhong> mat = std::make_shared<MaterialBlinnPhong>();
            planeMesh->material = mat;
            Plane::GeneratePlane(10, 10.0f, planeMesh);
            World::AddObject(planeObject);
        }

        ImGui::PopStyleVar(2);
        
        ImGui::EndPopup();
    }
    
    ImGui::PopStyleVar(1);
}

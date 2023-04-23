#include "Object.h"

#include "../Model.h"
#include "../components/Component.h"
#include "../components/Transform.h"
#include "../components/mesh/Mesh.h"

Object::Object()
{
    transform = CreateComponent<Transform>();
}

Object::~Object()
{
    for (const std::shared_ptr<Component>& component : components)
    {
        component->parent = nullptr;
    }
}

void Object::Tick() const
{
    for (const std::shared_ptr<Component>& component : components)
    {
        component->Tick();
    }
}

void Object::Draw() const
{
    const std::vector<std::shared_ptr<Mesh>> meshes = GetComponents<Mesh>();

    for (const std::shared_ptr<Mesh>& mesh : meshes)
    {
        if (mesh == nullptr)
        {
            continue;
        }

        mesh->Draw();
    }
}

void Object::LoadModel(const char* path)
{
    // May need to change this in the future in order to include whole scenes
    const std::vector<std::shared_ptr<Mesh>> meshes = Model::Load(path);
    for (const std::shared_ptr<Mesh>& mesh : meshes)
    {
        AddComponent(mesh);
    }
}

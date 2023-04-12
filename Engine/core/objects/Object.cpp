#include "Object.h"

#include "../components/Component.h"
#include "../components/Transform.h"
#include "../components/mesh/Mesh.h"
#include "../components/Model.h"

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

void Object::Draw() const
{
    const std::shared_ptr<Model> model = GetComponent<Model>();
    const std::shared_ptr<Mesh> mesh = GetComponent<Mesh>();

    if (model == nullptr && mesh == nullptr)
    {
        return;
    }

    if (model != nullptr)
    {
        model->Draw();
    }
    if (mesh != nullptr)
    {
        mesh->Draw();
    }
}

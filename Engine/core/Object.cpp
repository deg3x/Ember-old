#include "Object.h"

#include "Transform.h"
#include "Component.h"
#include "Material.h"
#include "Model.h"
#include "Mesh.h"

Object::Object()
{
    transform = Component::CreateDefaultComponent<Transform>();
    AddComponent(std::dynamic_pointer_cast<Component>(transform));
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

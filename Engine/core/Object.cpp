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
    const std::shared_ptr<Material> mat = GetComponent<Material>();
    const std::shared_ptr<Model> model = GetComponent<Model>();
    const std::shared_ptr<Mesh> mesh = GetComponent<Mesh>();

    if (mat == nullptr || (model == nullptr && mesh == nullptr))
    {
        return;
    }

    mat->Use();
    if (model != nullptr)
    {
        model->Draw();
    }
    if (mesh != nullptr)
    {
        mesh->DrawMesh();
    }
}

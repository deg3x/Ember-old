#include "Scene.h"

#include "Shader.h"
#include "Material.h"
#include "objects/Object.h"
#include "components/Transform.h"
#include "components/Camera.h"
#include "components/mesh/Mesh.h"
#include "components/light/Light.h"

#include <unordered_set>


void Scene::Tick() const
{
    Draw();
}

void Scene::AddObject(const std::shared_ptr<Object>& object)
{
    const std::shared_ptr<Camera> camComponent = object->GetComponent<Camera>();
    if (camComponent != nullptr)
    {
        camera = object;
    }

    const std::shared_ptr<Light> lightComponent = object->GetComponent<Light>();
    if (lightComponent != nullptr)
    {
        lights.push_back(object);
    }

    objects.push_back(object);
}

void Scene::Draw() const
{
    std::unordered_set<std::shared_ptr<Material>> sceneMats;
    
    for (const std::shared_ptr<Object>& object : objects)
    {
        const std::vector<std::shared_ptr<Mesh>> objectMeshes = object->GetComponents<Mesh>();

        for (const std::shared_ptr<Mesh>& mesh : objectMeshes)
        {
            sceneMats.insert(mesh->material);
        }
    }

    for (const std::shared_ptr<Material>& material : sceneMats)
    {
        for (const std::shared_ptr<Object>& light : lights)
        {
            light->GetComponent<Light>()->SetShaderProperties(*material->GetShader());
        }

        const std::shared_ptr<Camera> cameraComponent = camera->GetComponent<Camera>();

        //
        // The following is tied to our Phong shader. Find a way to generalize.
        //
        
        material->GetShader()->SetVector3("cameraPosition", camera->transform->position);
        material->GetShader()->SetMatrix4x4("view", cameraComponent->GetViewMatrix());
        material->GetShader()->SetMatrix4x4("projection", cameraComponent->GetProjectionMatrix());
        material->GetShader()->SetVector3("material.diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
        material->GetShader()->SetVector3("material.specular", glm::vec3(0.9f, 0.8f, 0.8f));
        material->GetShader()->SetFloat("material.shininess", 64);
    }

    for (const std::shared_ptr<Object>& object : objects)
    {
        object->Draw();
    }
}

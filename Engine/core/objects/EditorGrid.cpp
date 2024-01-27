#include "engine_pch.h"
#include "EditorGrid.h"

#include "glad/glad.h"

#include "core/Renderer.h"
#include "core/components/Mesh.h"
#include "core/materials/MaterialEditorGrid.h"
#include "utils/procedural/Plane.h"

EditorGrid::EditorGrid(const char* objName) : Object(objName)
{
    const std::shared_ptr<MaterialEditorGrid> gridMaterial = std::make_shared<MaterialEditorGrid>();

    const std::shared_ptr<Mesh> gridMesh = CreateComponent<Mesh>();
    Plane::GeneratePlane(10, 1.0f, gridMesh);
    gridMesh->material = gridMaterial;
}

void EditorGrid::Draw(const std::shared_ptr<Camera>& camera, const std::vector<std::shared_ptr<Light>>& lights) const
{
    Renderer::SetDepthTestFunc(GL_LEQUAL);
    Object::Draw(camera, lights);
    Renderer::SetDepthTestFunc(GL_LESS);
}

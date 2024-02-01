#include "engine_pch.h"
#include "EditorGrid.h"

#include "core/components/Mesh.h"
#include "core/Material.h"
#include "core/Object.h"
#include "core/Shader.h"
#include "core/World.h"
#include "core/components/Transform.h"
#include "utils/PathBuilder.h"
#include "utils/procedural/Plane.h"

std::shared_ptr<Object> EditorGrid::Instantiate()
{
    const std::string vertPath = PathBuilder::GetPath("./Engine/shaders/vertexEditorGrid.glsl");
    const std::string fragPath = PathBuilder::GetPath("./Engine/shaders/fragmentEditorGrid.glsl");
    
    const std::shared_ptr<Object> editorGrid = std::make_shared<Object>("Editor Grid");
    const std::shared_ptr<Shader> gridShader = std::make_shared<Shader>(vertPath.c_str(), fragPath.c_str());
    const std::shared_ptr<Material> gridMat  = std::make_shared<Material>(gridShader);
    const std::shared_ptr<Mesh> gridMesh     = editorGrid->CreateComponent<Mesh>();
    
    Plane::GeneratePlane(10, 100.0f, gridMesh);
    
    gridMesh->material           = gridMat;
    gridMesh->meshType           = MeshType::TRANSPARENT;
    gridMesh->cullingMode        = CullingMode::NONE;
    gridMesh->writeToDepthBuffer = false;

    // We push the grid lower to avoid Z-fighting
    editorGrid->transform->position = {0.0f, -0.001f, 0.0f};

    World::AddObject(editorGrid);

    return editorGrid;
}

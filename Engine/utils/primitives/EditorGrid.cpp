#include "engine_pch.h"
#include "EditorGrid.h"

#include "core/components/Mesh.h"
#include "core/materials/MaterialEditorGrid.h"
#include "core/Object.h"
#include "core/World.h"
#include "utils/procedural/Plane.h"

std::shared_ptr<Object> EditorGrid::Instantiate()
{
    const std::shared_ptr<Object> editorGrid = std::make_shared<Object>("Editor Grid");
    const std::shared_ptr<MaterialEditorGrid> gridMaterial = std::make_shared<MaterialEditorGrid>();
    const std::shared_ptr<Mesh> gridMesh = editorGrid->CreateComponent<Mesh>();
    
    Plane::GeneratePlane(10, 100.0f, gridMesh);
    
    gridMesh->material           = gridMaterial;
    gridMesh->meshType           = MeshType::TRANSPARENT;
    gridMesh->cullingMode        = CullingMode::NONE;
    gridMesh->writeToDepthBuffer = false;

    World::AddObject(editorGrid);

    return editorGrid;
}

#include "EditorGrid.h"

#include <glad/glad.h>

#include "../Renderer.h"
#include "../components/meshes/Plane.h"
#include "../materials/MaterialEditorGrid.h"

#include <memory>

EditorGrid::EditorGrid()
{
    const std::shared_ptr<MaterialEditorGrid> material = std::make_shared<MaterialEditorGrid>();

    CreateComponent<Plane>(material);
}

void EditorGrid::Draw(const std::shared_ptr<Camera>& camera, const std::vector<std::shared_ptr<Light>>& lights) const
{
    Renderer::SetDepthTestFunc(GL_LEQUAL);
    Object::Draw(camera, lights);
    Renderer::SetDepthTestFunc(GL_LESS);
}

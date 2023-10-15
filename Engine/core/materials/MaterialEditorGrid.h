#pragma once

#include "Material.h"

class ENGINE_API MaterialEditorGrid : public Material
{
public:
    MaterialEditorGrid();
    
    void Use() const override;
    void SetupShaderVariables(const Transform& objectTransform, const Camera& camera) const override;
};

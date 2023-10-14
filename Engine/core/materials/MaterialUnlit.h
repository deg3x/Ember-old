#pragma once

#include "Material.h"
#include "glm/glm.hpp"

class ENGINE_API MaterialUnlit : public Material
{
public:
    glm::vec4 diffuseColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    
public:
    MaterialUnlit();
    
    void Use() const override;
    void SetupShaderVariables(const Transform& objectTransform, const Camera& camera) const override;
};

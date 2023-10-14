#pragma once

#include "Material.h"
#include "glm/glm.hpp"

class TextureDiffuse;

class ENGINE_API MaterialBlinnPhong : public Material
{
public:
    glm::vec4 diffuseColor  = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    glm::vec3 specular      = glm::vec3(0.9f, 0.8f, 0.8f);
    float shininessExponent = 64.0f;
    
private:
    std::shared_ptr<TextureDiffuse> diffuseTexture;
    
public:
    MaterialBlinnPhong();
    MaterialBlinnPhong(const std::shared_ptr<TextureDiffuse>& texture);
    
    void Use() const override;
    void SetupShaderVariables(const Transform& objectTransform, const Camera& camera) const override;

    void SetDiffuseTexture(const std::shared_ptr<TextureDiffuse>& texture)
    {
        diffuseTexture = texture;
    }
};

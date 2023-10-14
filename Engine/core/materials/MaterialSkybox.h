#pragma once

#include "Material.h"

class Texture;

class ENGINE_API MaterialSkybox : public Material
{
private:
    std::shared_ptr<Texture> cubemap;
    
public:
    MaterialSkybox();
    MaterialSkybox(const std::shared_ptr<Texture> texture);
    
    void Use() const override;
    void SetupShaderVariables(const Transform& objectTransform, const Camera& camera) const override;

    void SetCubemap(const std::shared_ptr<Texture>& texture)
    {
        cubemap = texture;
    }
};

#pragma once

#include "Material.h"

class TextureCubemap;

class ENGINE_API MaterialSkybox : public Material
{
private:
    std::shared_ptr<TextureCubemap> cubemap;
    
public:
    MaterialSkybox();
    MaterialSkybox(const std::shared_ptr<TextureCubemap> texture);
    
    void Use() const override;
    void SetupShaderVariables(const Transform& objectTransform, const Camera& camera) const override;

    void SetCubemap(const std::shared_ptr<TextureCubemap>& texture)
    {
        cubemap = texture;
    }
};

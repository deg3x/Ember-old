#pragma once

#include "Definitions.h"

class Shader;
class Camera;
class Transform;

class ENGINE_API Material
{
protected:
    std::shared_ptr<Shader> shader;

public:
    Material() {}
    virtual ~Material() {}

    virtual void Use() const = 0;
    virtual void SetupShaderVariables(const Transform& objectTransform, const Camera& camera) const = 0;
    
    inline std::shared_ptr<Shader> GetShader() const
    {
        return shader;
    }
};
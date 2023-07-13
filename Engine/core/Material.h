#pragma once

#include "../Definitions.h"
#include "../utils/Types.h"

#include <memory>
#include <vector>

class Shader;
class Texture;
class Camera;
class Transform;

class ENGINE_API Material
{
protected:
    MaterialType type;
    MaterialProperties* properties;
    std::shared_ptr<Shader> shader;
    std::vector<std::shared_ptr<Texture>> textures;

public:
    Material();
    Material(const char* vertShader, const char* fragShader, MaterialType matType);
    Material(const char* vertShader, const char* fragShader, MaterialType matType, const std::vector<std::shared_ptr<Texture>>& diffTextures);
    virtual ~Material();

    void SetShader(const char* vertShader, const char* fragShader);
    void SetTextures(const std::vector<std::shared_ptr<Texture>>& diffTextures);
    void SetProperties(const MaterialProperties* matProperties);

    void Use() const;
    void SetupShaderVariables(const Transform& objectTransform, const Camera& camera) const;

    inline MaterialType GetType() const
    {
        return type;
    }
    
    inline const MaterialProperties* GetProperties() const
    {
        return properties;
    }
    
    inline std::shared_ptr<Shader> GetShader() const
    {
        return shader;
    }

    inline std::vector<std::shared_ptr<Texture>> GetTextures() const
    {
        return textures;
    }

protected:
};
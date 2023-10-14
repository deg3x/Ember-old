#pragma once

#include "../Definitions.h"
#include "glm/glm.hpp"
#include <memory>
#include <vector>

enum class MaterialType
{
    Unlit,
    Phong,
    Cubemap
};

struct MaterialProperties
{
    MaterialProperties() = default;
    virtual ~MaterialProperties() = default;
};

struct MaterialPropertiesUnlit : public MaterialProperties
{
    glm::vec4 diffuse = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	
    MaterialPropertiesUnlit() : MaterialProperties() {}
};

struct MaterialPropertiesPhong : public MaterialProperties
{
    glm::vec4 diffuse       = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    glm::vec3 specular      = glm::vec3(0.9f, 0.8f, 0.8f);
    float shininessExponent = 64.0f;

    MaterialPropertiesPhong() : MaterialProperties() {}
};

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
    void SetTexture(const std::shared_ptr<Texture>& texture);
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
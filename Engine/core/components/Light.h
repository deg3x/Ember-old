#pragma once

#include "LightTypes.h"
#include "core/components/Component.h"

class Shader;



class ENGINE_API Light : public Component
{
    MAKE_COMPONENT_TYPE(LIGHT)
    
public:
    DirectionalLightData directional;
    PointLightData point;
    SpotLightData spot;
    LightType type;

public:
    Light();

    void Tick() override;
    
    void SetShaderProperties(const Shader& shaderProgram, int lightIndex) const;
};
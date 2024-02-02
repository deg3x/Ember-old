#version 410 core

in vec3 Normal;
in vec2 TexCoord;
in vec3 WorldPosition;

out vec4 FragmentColor;

#include "includes/lighting.glsl"
#include "includes/PBR.glsl"

uniform vec3 cameraPosition;

void main()
{
    vec3 normVector = normalize(Normal);
    vec3 viewVector = normalize(cameraPosition - WorldPosition);
    vec3 irradiance = vec3(0.0);
    
    for (int i = 0; i < activeLightsDir; i++)
    {
        
    }
    for (int i = 0; i < activeLightsPoint; i++)
    {
        vec3 lightVector  = normalize(pointLight[i].position - WorldPosition);
        vec3 halfVector   = normalize(viewVector + lightVector);
        float distance    = length(pointLight[i].position - WorldPosition);
        float attenuation = 1.0 / (distance * distance);
        
        vec3 radiance = pointLight[i].color * attenuation;
        
        irradiance += PBREquationComponent(normVector, viewVector, lightVector, halfVector, radiance);
    }
    for (int i = 0; i < activeLightsSpot; i++)
    {
        
    }
    
    vec3 ambient = vec3(0.03) * albedo * ambientOcclusion;
    vec3 color   = ambient + irradiance;
    
    // HDR mapping and Gamma correction
    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0/2.2));
    
    FragmentColor = vec4(color, 1.0);
}
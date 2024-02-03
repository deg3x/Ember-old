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
    
    vec3 albedoVal     = hasMapAlbedo ? texture(albedoMap, TexCoord).rgb : albedo;
    float metallicVal  = hasMapMetallic ? texture(metallicMap, TexCoord).r : metallic;
    float roughnessVal = hasMapRoughness ? texture(roughnessMap, TexCoord).r : roughness;
    float ao           = !hasMapAmbientOcclusion ? texture(ambientOcclusionMap, TexCoord).r : ambientOcclusion;
    
    for (int i = 0; i < activeLightsDir; i++)
    {
        vec3 lightVector = normalize(-dirLights[i].direction);
        vec3 halfVector  = normalize(viewVector + lightVector);
        
        vec3 radiance = dirLights[i].color * CalculateLightDirectional(dirLights[i]);
        
        irradiance += PBREquationComponent(normVector, viewVector, lightVector, halfVector, radiance, albedoVal, metallicVal, roughnessVal);
    }
    for (int i = 0; i < activeLightsPoint; i++)
    {
        vec3 lightVector = normalize(pointLights[i].position - WorldPosition);
        vec3 halfVector  = normalize(viewVector + lightVector);
        
        vec3 radiance = pointLights[i].color * CalculateLightPoint(pointLights[i], WorldPosition);
        
        irradiance += PBREquationComponent(normVector, viewVector, lightVector, halfVector, radiance, albedoVal, metallicVal, roughnessVal);
    }
    for (int i = 0; i < activeLightsSpot; i++)
    {
        vec3 lightVector = normalize(spotLights[i].position - WorldPosition);
        vec3 halfVector  = normalize(viewVector + lightVector);
        
        vec3 radiance = spotLights[i].color * CalculateLightSpot(spotLights[i], WorldPosition);
        
        irradiance += PBREquationComponent(normVector, viewVector, lightVector, halfVector, radiance, albedoVal, metallicVal, roughnessVal);
    }
    
    vec3 ambient = vec3(0.03) * albedoVal * ao;
    vec3 color   = ambient + irradiance;
    
    // HDR mapping and Gamma correction
    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0/2.2));
    
    FragmentColor = vec4(color, 1.0);
}
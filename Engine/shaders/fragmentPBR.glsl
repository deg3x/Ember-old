#version 410 core

in vec2 TexCoord;
in vec3 WorldPosition;
in mat3 TBN;

out vec4 FragmentColor;

#include "includes/common.glsl"
#include "includes/lighting.glsl"
#include "includes/PBR.glsl"

uniform vec3 cameraPosition;

uniform samplerCube irradianceMap;
uniform bool hasIrradianceMap;

uniform sampler2D albedoMap;
uniform sampler2D normalMap;
uniform sampler2D metallicMap;
uniform sampler2D roughnessMap;
uniform sampler2D ambientOcclusionMap;

uniform bool hasMapAlbedo           = false;
uniform bool hasMapNormal           = false;
uniform bool hasMapMetallic         = false;
uniform bool hasMapRoughness        = false;
uniform bool hasMapAmbientOcclusion = false;

uniform vec3 albedo;
uniform float metallic;
uniform float roughness;
uniform float ambientOcclusion;

void main()
{
    vec3 viewVector = normalize(cameraPosition - WorldPosition);
    vec3 irradiance = vec3(0.0);
    
    vec3 albedoVal     = hasMapAlbedo ? pow(texture(albedoMap, TexCoord).rgb, vec3(2.2)) : albedo;
    vec3 normVector    = hasMapNormal ? SampleNormalMap(normalMap, TexCoord, TBN) : normalize(TBN[2]);
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
    
    vec3 ambient = vec3(0.03) * albedoVal;
    
    // Indirect/Image lighting
    if (hasIrradianceMap)
    {
        vec3 specular = FresnelSchlickRoughness(max(dot(normal, viewVector), 0.0), albedoVal, metallicVal, roughnessVal);
        vec3 diffuse  = 1.0 - specular;
        
        vec3 envIrradiance = texture(irradianceMap, normal).rgb;
        
        ambient = envIrradiance * albedoVal * diffuse;
    }
    
    ambient = ambient * ao;
    vec3 color   = ambient + irradiance;
    
    // HDR mapping and Gamma correction
    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0/2.2));
    
    FragmentColor = vec4(color, 1.0);
}
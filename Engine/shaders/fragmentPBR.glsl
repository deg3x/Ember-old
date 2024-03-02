#version 410 core

in vec2 TexCoord;
in vec3 WorldPos;
in mat3 TBN;

out vec4 FragmentColor;

#include "includes/common.glsl"
#include "includes/lighting.glsl"
#include "includes/PBR.glsl"

uniform vec3 cameraPosition;

uniform samplerCube irradianceMap;
uniform samplerCube prefilterMap;
uniform sampler2D brdfMap;

uniform bool hasImageBasedLighting = false;

uniform sampler2D albedoMap;
uniform sampler2D normalMap;
uniform sampler2D metallicMap;
uniform sampler2D roughnessMap;
uniform sampler2D ambientOcclusionMap;
uniform sampler2D emissionMap;

uniform bool hasMapAlbedo           = false;
uniform bool hasMapNormal           = false;
uniform bool hasMapMetallic         = false;
uniform bool hasMapRoughness        = false;
uniform bool hasMapAmbientOcclusion = false;
uniform bool hasMapEmission         = false;

uniform vec3 albedo            = vec3(1.0, 1.0, 1.0);
uniform float metallic         = 1.0;
uniform float roughness        = 0.0;
uniform float ambientOcclusion = 0.1;

void main()
{
    vec3 viewVector = normalize(cameraPosition - WorldPos);
    vec3 irradiance = vec3(0.0);
    
    vec3 albedoVal     = hasMapAlbedo ? pow(texture(albedoMap, TexCoord).rgb, vec3(2.2)) : albedo;
    vec3 normVector    = hasMapNormal ? SampleNormalMap(normalMap, TexCoord, TBN) : normalize(TBN[2]);
    float metallicVal  = hasMapMetallic ? texture(metallicMap, TexCoord).r : metallic;
    float roughnessVal = hasMapRoughness ? texture(roughnessMap, TexCoord).g : roughness;
    float ao           = hasMapAmbientOcclusion ? texture(ambientOcclusionMap, TexCoord).r : ambientOcclusion;
    
    for (int i = 0; i < activeLightsDir; i++)
    {
        vec3 lightVector = normalize(-dirLights[i].direction);
        vec3 halfVector  = normalize(viewVector + lightVector);
        
        vec3 radiance = dirLights[i].color * CalculateLightDirectional(dirLights[i]);
        
        irradiance += PBREquationComponent(normVector, viewVector, lightVector, halfVector, radiance, albedoVal, metallicVal, roughnessVal);
    }
    for (int i = 0; i < activeLightsPoint; i++)
    {
        vec3 lightVector = normalize(pointLights[i].position - WorldPos);
        vec3 halfVector  = normalize(viewVector + lightVector);
        
        vec3 radiance = pointLights[i].color * CalculateLightPoint(pointLights[i], WorldPos);
        
        irradiance += PBREquationComponent(normVector, viewVector, lightVector, halfVector, radiance, albedoVal, metallicVal, roughnessVal);
    }
    for (int i = 0; i < activeLightsSpot; i++)
    {
        vec3 lightVector = normalize(spotLights[i].position - WorldPos);
        vec3 halfVector  = normalize(viewVector + lightVector);
        
        vec3 radiance = spotLights[i].color * CalculateLightSpot(spotLights[i], WorldPos);
        
        irradiance += PBREquationComponent(normVector, viewVector, lightVector, halfVector, radiance, albedoVal, metallicVal, roughnessVal);
    }
    
    ////// Indirect/Image lighting
    vec3 baseReflectivity = vec3(0.04);
    float dotNormView     = max(dot(normVector, viewVector), 0.0);
    
    // Diffuse
    vec3 specular      = FresnelSchlickRoughness(dotNormView, albedoVal, metallicVal, baseReflectivity, roughnessVal);
    vec3 diffuse       = (1.0 - specular) * (1.0 - metallicVal);
    vec3 envIrradiance = texture(irradianceMap, normVector).rgb;
    
    // Specular
    const float maxReflectionLOD = 4.0;
    
    vec3 reflection     = reflect(-viewVector, normVector);
    vec3 prefilterColor = textureLod(prefilterMap, reflection, roughnessVal * maxReflectionLOD).rgb;
    vec3 fresnel        = FresnelSchlickRoughness(dotNormView, albedoVal, metallicVal, baseReflectivity, roughnessVal);
    vec2 envBRDF        = texture(brdfMap, vec2(dotNormView, roughnessVal)).rg;
    vec3 envSpecular    = prefilterColor * (fresnel * envBRDF.x + envBRDF.y);
    
    
    vec3 ambient = hasImageBasedLighting ? (envIrradiance * albedoVal * diffuse + envSpecular) * ao : (vec3(0.03) * albedoVal) * ao;
    
    vec3 color = ambient + irradiance;
    
    vec3 emissionVal = hasMapEmission ? texture(emissionMap, TexCoord).rgb : vec3(0.0);
    
    // HDR mapping and Gamma correction
    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0/2.2));
    
    // Add emission after HDR + gamma
    color += emissionVal;
    
    FragmentColor = vec4(color, 1.0);
}

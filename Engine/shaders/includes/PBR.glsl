const float PI = 3.1415926538;

uniform sampler2D albedoMap;
uniform sampler2D metallicMap;
uniform sampler2D roughnessMap;
uniform sampler2D ambientOcclusionMap;

uniform bool hasMapAlbedo;
uniform bool hasMapMetallic;
uniform bool hasMapRoughness;
uniform bool hasMapAmbientOcclusion;

uniform vec3 albedo;
uniform float metallic;
uniform float roughness;
uniform float ambientOcclusion;

vec3 FresnelSchlick(float dotHalfView, vec3 albedoVal, float metallicVal)
{
    vec3 baseRefl = vec3(0.04);
    baseRefl      = mix(baseRefl, albedoVal, metallicVal);
    
    vec3 fresnel  = baseRefl + (1.0 - baseRefl) * pow(clamp(1.0 - dotHalfView, 0.0, 1.0), 5.0);
    
    return fresnel;
}

float GeometrySchlickGGX(float dotNormalView, float roughnessVal)
{
    float kappa = ((roughnessVal + 1.0) * (roughnessVal + 1.0)) / 8.0;
    
    return dotNormalView / (dotNormalView * (1.0 - kappa) + kappa);
}

float GeometrySmith(float dotNormalView, float dotNormalLight, float roughnessVal)
{
    float ggx1 = GeometrySchlickGGX(dotNormalView, roughnessVal);
    float ggx2 = GeometrySchlickGGX(dotNormalLight, roughnessVal);
    
    return ggx1 * ggx2;
}

float NDFTrowbridgeReitz(float dotNormalHalf, float roughnessVal)
{
    float alpha       = roughnessVal * roughnessVal;
    float alphaSqr    = alpha * alpha;
    float dotSqr      = dotNormalHalf * dotNormalHalf;
    float denominator = (dotSqr * (alphaSqr - 1.0) + 1.0);
    denominator       = PI * denominator * denominator;
    
    return alphaSqr / denominator;
}

vec3 PBREquationComponent(vec3 normal, vec3 viewVector, vec3 lightVector, vec3 halfVector, vec3 lightRadiance, vec3 albedoVal, float metallicVal, float roughnessVal)
{
    float dotNormalView  = max(dot(normal, viewVector), 0.0);
    float dotNormalLight = max(dot(normal, lightVector), 0.0);
    float dotHalfView    = dot(halfVector, viewVector);
    float dotNormalHalf  = max(dot(normal, halfVector), 0.0);
    
    vec3 fresnel   = FresnelSchlick(dotHalfView, albedoVal, metallicVal);
    float geometry = GeometrySmith(dotNormalView, dotNormalLight, roughnessVal);
    float NDF      = NDFTrowbridgeReitz(dotNormalHalf, roughnessVal);
    
    vec3 numerator    = NDF * geometry * fresnel;
    float denominator = 4.0 * dotNormalView * dotNormalLight + 0.0001;
    
    vec3 BRDF = numerator / denominator;
    
    vec3 kSpecular = fresnel;
    vec3 kDiffuse  = (vec3(1.0) - kSpecular) * (1.0 - metallicVal);
    
    return ((kDiffuse * albedoVal / PI) + BRDF) * lightRadiance * dotNormalLight;
}
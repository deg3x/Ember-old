const float PI = 3.1415926538;

vec3 FresnelSchlick(float dotHalfView, vec3 albedoVal, float metallicVal, vec3 baseReflectivity)
{
    baseReflectivity = mix(baseReflectivity, albedoVal, metallicVal);
    
    vec3 fresnel  = baseReflectivity + (1.0 - baseReflectivity) * pow(clamp(1.0 - dotHalfView, 0.0, 1.0), 5.0);
    
    return fresnel;
}

vec3 FresnelSchlickRoughness(float dotHalfView, vec3 albedoVal, float metallicVal, vec3 baseReflectivity, float roughness)
{
    baseReflectivity = mix(baseReflectivity, albedoVal, metallicVal);
    
    vec3 fresnel  = baseReflectivity + (max(vec3(1.0 - roughness), baseReflectivity) - baseReflectivity) * pow(clamp(1.0 - dotHalfView, 0.0, 1.0), 5.0);
    
    return fresnel;
}

float GeometrySchlickGGX(float dotNormalView, float roughnessVal)
{
    float kappa = ((roughnessVal + 1.0) * (roughnessVal + 1.0)) / 8.0;
    
    return dotNormalView / (dotNormalView * (1.0 - kappa) + kappa);
}

float GeometrySchlickIBLGGX(float dotNormalView, float roughnessVal)
{
    float kappa = (roughnessVal * roughnessVal) / 2.0;
    
    return dotNormalView / (dotNormalView * (1.0 - kappa) + kappa);
}

vec3 ImportanceSampleGGX(vec2 vector, vec3 normal, float roughnessVal)
{
    float alpha    = roughnessVal * roughnessVal;
    float alphaSqr = alpha * alpha;
    
    float phi      = 2.0 * PI * vector.x;
    float cosTheta = sqrt((1.0 - vector.x) / (1.0 + (alphaSqr - 1.0) * vector.y));
    float sinTheta = sqrt(1.0 - cosTheta * cosTheta);
    
    vec3 point;
    point.x = cos(phi) * sinTheta;
    point.y = sin(phi) * sinTheta;
    point.z = cosTheta;
    
    vec3 up        = abs(normal.z) < 0.999 ? vec3(0.0, 0.0, 1.0) : vec3(1.0, 0.0, 0.0);
    vec3 tangent   = normalize(cross(up, normal));
    vec3 bitangent = cross(normal, tangent);
    
    vec3 sampleVector = tangent * point.x + bitangent * point.y + normal * point.z;
    
    return normalize(sampleVector);
}

float GeometrySmith(float dotNormalView, float dotNormalLight, float roughnessVal)
{
    float ggx1 = GeometrySchlickGGX(dotNormalView, roughnessVal);
    float ggx2 = GeometrySchlickGGX(dotNormalLight, roughnessVal);
    
    return ggx1 * ggx2;
}

float GeometrySmithIBL(float dotNormalView, float dotNormalLight, float roughnessVal)
{
    float ggx1 = GeometrySchlickIBLGGX(dotNormalView, roughnessVal);
    float ggx2 = GeometrySchlickIBLGGX(dotNormalLight, roughnessVal);
    
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
    
    vec3 fresnel   = FresnelSchlick(dotHalfView, albedoVal, metallicVal, vec3(0.04));
    float geometry = GeometrySmith(dotNormalView, dotNormalLight, roughnessVal);
    float NDF      = NDFTrowbridgeReitz(dotNormalHalf, roughnessVal);
    
    vec3 numerator    = NDF * geometry * fresnel;
    float denominator = 4.0 * dotNormalView * dotNormalLight + 0.0001;
    
    vec3 BRDF = numerator / denominator;
    
    vec3 kSpecular = fresnel;
    vec3 kDiffuse  = (vec3(1.0) - kSpecular) * (1.0 - metallicVal);
    
    return ((kDiffuse * albedoVal / PI) + BRDF) * lightRadiance * dotNormalLight;
}
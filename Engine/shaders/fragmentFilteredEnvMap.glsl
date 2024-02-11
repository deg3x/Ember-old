#version 410

out vec4 FragmentColor;

in vec3 localPos;

uniform samplerCube environmentMap;
uniform float roughness;

#include "./includes/common.glsl"
#include "./includes/PBR.glsl"

void main()
{
    vec3 normal = normalize(localPos);
    vec3 refl   = normal;
    vec3 view   = normal;
    
    const uint nSamples = 1024u;
    float totalWeight   = 0.0;
    vec3 outputColor    = vec3(0.0);
    
    for (uint i = 0u; i < nSamples; i++)
    {
        vec2 vector = HammersleySequence(i, nSamples);
        vec3 halfV  = ImportanceSampleGGX(vector, normal, roughness);
        vec3 light  = normalize(2.0 * dot(view, halfV) * halfV - view);
        
        float dotNormalLight = max(dot(normal, light), 0.0);
        if (dotNormalLight > 0.0)
        {
            outputColor += texture(environmentMap, light).rgb * dotNormalLight;
            totalWeight      += dotNormalLight;
        }
    }
    
    outputColor = outputColor / totalWeight;
    
    FragmentColor = vec4(outputColor, 1.0);
}
#version 410

out vec2 FragmentColor;

in vec2 TexCoords;

#include "./includes/common.glsl"
#include "./includes/PBR.glsl"

void main()
{
    float dotNormalView = TexCoords.x;
    float roughness     = TexCoords.y;
    
    vec3 view;
    view.x = sqrt(1.0 - dotNormalView * dotNormalView);
    view.y = 0.0;
    view.z = dotNormalView;
    
    float A = 0.0;
    float B = 0.0;
    
    vec3 planeNorm = vec3(0.0, 0.0, 1.0);
    
    const uint nSamples = 1024u;
    for (uint i = 0u; i < nSamples; i++)
    {
        vec2 vector = HammersleySequence(i, nSamples);
        vec3 halfV  = ImportanceSampleGGX(vector, planeNorm, roughness);
        vec3 light  = normalize(2.0 * dot(view, halfV) * halfV - view);
        
        float dotNormalLight = max(light.z, 0.0);
        float dotNormalHalf  = max(halfV.z, 0.0);
        float dotViewHalf   = max(dot(view, halfV), 0.0);
        
        if (dotNormalLight > 0.0)
        {
            float geometry    = GeometrySmithIBL(dotNormalView, dotNormalLight, roughness);
            float geometryVis = (geometry * dotViewHalf) / (dotNormalHalf * dotNormalView);
            float fresnel     = pow(1.0 - dotViewHalf, 5.0);
            
            A += (1.0 - fresnel) * geometryVis;
            B += fresnel * geometryVis;
        } 
    }
    
    A /= float(nSamples);
    B /= float(nSamples);
    
    FragmentColor = vec2(A, B);
}
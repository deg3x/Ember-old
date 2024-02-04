vec3 SampleNormalMap(sampler2D map, vec2 uv, mat3 tbnMatrix)
{
    vec3 normal = normalize(texture(map, uv).rgb) * 2.0 - 1.0;
    
    vec3 tangent    = TBN[0];
    vec3 bitangent  = TBN[1];
    vec3 calcNormal = cross(tangent, bitangent);
    
    float normalCheck = dot(calcNormal, TBN[2]);
    
    if (normalCheck < 0.5)
    {
        tbnMatrix[0] = -tbnMatrix[0];
        normal = normalize(tbnMatrix * normal);
    }
    else
    {
        normal = normalize(tbnMatrix * normal);
    }
    
    return normal;
}
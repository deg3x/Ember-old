vec3 SampleNormalMap(sampler2D map, vec2 uv, mat3 tbnMatrix)
{
    vec3 normal = normalize(texture(map, uv).rgb * 2.0 - 1.0);
    
    vec3 tangent    = tbnMatrix[0];
    vec3 bitangent  = tbnMatrix[1];
    vec3 calcNormal = cross(tangent, bitangent);
    
    float normalCheck = dot(calcNormal, tbnMatrix[2]);
    
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

const vec2 invAtan = vec2(0.1591, 0.3183);
vec2 SampleSphericalMap(vec3 vector)
{
    vec2 uv = vec2(atan(vector.z, vector.x), asin(vector.y));
    uv *= invAtan;
    uv += 0.5;
    
    return uv;
}

float RadicalInverseVanDerCorput(uint bits)
{
    bits = (bits << 16u) | (bits >> 16u);
    bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
    bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
    bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
    bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
    
    return float(bits) * 2.3283064365386963e-10;
}

vec2 HammersleySequence(uint smpl, uint population)
{
    return vec2(float(smpl) / float(population), RadicalInverseVanDerCorput(smpl));
}
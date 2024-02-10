#version 410 core

in vec3 localPos;

out vec4 FragmentColor;

uniform sampler2D equirectMap;

#include "./includes/common.glsl"

void main()
{
    vec2 uv    = SampleSphericalMap(normalize(localPos));
    vec3 color = texture(equirectMap, uv).rgb;
    
    FragmentColor = vec4(color, 1.0);
}
#version 410 core

in vec3 WorldPos;

out vec4 FragColor;

void main()
{
    float thickness = 0.03f;
    
    vec2 gridTest = step(abs(fract(WorldPos.xz)), vec2(thickness));
    FragColor = vec4(0.8, 0.8, 0.8, clamp(gridTest.x + gridTest.y, 0.0, 0.9));
}
#version 410 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;

out vec2 TexCoords;

void main()
{
    TexCoords   = aTexCoord;
    gl_Position = vec4(aPos, 1.0);
}
#version 410 core

layout(location = 0) in vec3 aPosition;

out vec3 WorldPos;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
    WorldPos = vec3(model * vec4(aPosition.x, aPosition.y, aPosition.z, 1.0));
    gl_Position = projection * view * vec4(WorldPos, 1.0);
}
#version 410 core

layout (location = 0) in vec3 aPos;

out vec3 TexCoords;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    TexCoords = aPos;
    vec4 originalPosition = projection * view * vec4(aPos, 1.0);
    gl_Position = originalPosition.xyww; // Set z = w so that we get the max depth value (1.0) after perspective division
}

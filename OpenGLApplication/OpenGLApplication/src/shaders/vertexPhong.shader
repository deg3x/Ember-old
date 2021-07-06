#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;

out vec3 Normal;
out vec3 WorldPos;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform mat4 normalMatrix;
uniform float time;

void main()
{
	gl_Position = projection * view * model * vec4(aPos.x, aPos.y, aPos.z, 1.0);
	WorldPos = vec3(model * vec4(aPos.x, aPos.y, aPos.z, 1.0));
	Normal = mat3(normalMatrix) * aNormal;
}
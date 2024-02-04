#version 410 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;
layout(location = 3) in vec3 aTangent;
layout(location = 4) in vec3 aBitangent;

out vec3 Normal;
out vec2 TexCoord;
out vec3 WorldPos;
out mat3 TBN;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform mat4 normalMatrix;

void main()
{
	gl_Position = projection * view * model * vec4(aPosition.x, aPosition.y, aPosition.z, 1.0);
	WorldPos    = vec3(model * vec4(aPosition.x, aPosition.y, aPosition.z, 1.0));
	Normal      = normalize(vec3(normalMatrix * vec4(aNormal, 0.0)));
	TexCoord    = vec2(aTexCoord.x, aTexCoord.y);
	
	vec3 Tangent   = normalize(vec3(normalMatrix * vec4(aTangent, 0)));
	vec3 Bitangent = normalize(vec3(normalMatrix * vec4(aBitangent, 0)));
	
	TBN = mat3(Tangent, Bitangent, Normal);
}
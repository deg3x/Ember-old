#version 410 core

in vec3 Normal;
in vec2 TexCoord;
in vec3 WorldPosition;

out vec4 FragmentColor;

#include "lighting.glsl"

uniform vec3 cameraPosition;
uniform sampler2D diffuseTexture;

void main()
{
	vec3 normal = normalize(Normal);
	vec3 viewDirection = normalize(cameraPosition - WorldPosition);

	vec3 color = vec3(0.0, 0.0, 0.0);
	
	for (int i = 0; i < activeLightsDir; i++)
	{
	    color += CalculateDirectionalLight(directionalLight[i], normal, viewDirection);
	}
	for (int i = 0; i < activeLightsPoint; i++)
	{
	    color += CalculatePointLight(pointLight[i], normal, viewDirection);
	}
	for (int i = 0; i < activeLightsSpot; i++)
	{
	    color += CalculateSpotLight(spotLight[i], normal, viewDirection);
	}
	
	/*if (directionalLight.isUsed)
	    color += CalculateDirectionalLight(directionalLight, normal, viewDirection);
	
	if (pointLight.isUsed)
	    color += CalculatePointLight(pointLight, normal, viewDirection);
	
	if (spotLight.isUsed)
	    color += CalculateSpotLight(spotLight, normal, viewDirection);*/

	FragmentColor = texture(diffuseTexture, TexCoord) * vec4(color, material.diffuse.w);
}



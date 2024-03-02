#version 410 core

in vec2 TexCoord;
in vec3 WorldPos;
in mat3 TBN;

out vec4 FragmentColor;

#include "includes/lighting.glsl"
#include "includes/BlinnPhong.glsl"

uniform vec3 cameraPosition;
uniform sampler2D diffuseTexture;

void main()
{
	vec3 normal = normalize(TBN[2]);
	vec3 viewVector = normalize(cameraPosition - WorldPos);

	vec3 color = vec3(0.0, 0.0, 0.0);
	
	for (int i = 0; i < activeLightsDir; i++)
	{
	    vec3 DSA = BlinnPhongComponents(normal, viewVector, dirLights[i].direction, dirLights[i].color, dirLights[i].ambient);
	    float lightEffect = CalculateLightDirectional(dirLights[i]);
	    
	    color += DSA * lightEffect;
	}
	for (int i = 0; i < activeLightsPoint; i++)
	{
	    vec3 lightDir = WorldPos - pointLights[i].position;
	    vec3 DSA = BlinnPhongComponents(normal, viewVector, lightDir, pointLights[i].color, pointLights[i].ambient);
	    float lightEffect = CalculateLightPoint(pointLights[i], WorldPos);
	    
	    color += DSA * lightEffect;
	}
	for (int i = 0; i < activeLightsSpot; i++)
	{
	    vec3 lightDir = WorldPos - spotLights[i].position;
	    vec3 DSA = BlinnPhongComponents(normal, viewVector, lightDir, spotLights[i].color, spotLights[i].ambient);
	    float lightEffect = CalculateLightSpot(spotLights[i], WorldPos);
	    
	    color += DSA * lightEffect;
	}

	FragmentColor = texture(diffuseTexture, TexCoord) * vec4(color, material.diffuse.w);
}



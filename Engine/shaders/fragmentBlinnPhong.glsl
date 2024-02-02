#version 410 core

in vec3 Normal;
in vec2 TexCoord;
in vec3 WorldPosition;

out vec4 FragmentColor;

#include "includes/lighting.glsl"
#include "includes/BlinnPhong.glsl"

uniform vec3 cameraPosition;
uniform sampler2D diffuseTexture;

void main()
{
	vec3 normal = normalize(Normal);
	vec3 viewVector = normalize(cameraPosition - WorldPosition);

	vec3 color = vec3(0.0, 0.0, 0.0);
	
	for (int i = 0; i < activeLightsDir; i++)
	{
	    vec3 DSA = BlinnPhongComponents(normal, viewVector, directionalLight[i].direction, directionalLight[i].color, directionalLight[i].ambient);
	    float lightEffect = CalculateLightDirectional(directionalLight[i]);
	    
	    color += DSA * lightEffect;
	}
	for (int i = 0; i < activeLightsPoint; i++)
	{
	    vec3 lightDir = WorldPosition - pointLight[i].position;
	    vec3 DSA = BlinnPhongComponents(normal, viewVector, lightDir, pointLight[i].color, pointLight[i].ambient);
	    float lightEffect = CalculateLightPoint(pointLight[i], WorldPosition);
	    
	    color += DSA * lightEffect;
	}
	for (int i = 0; i < activeLightsSpot; i++)
	{
	    vec3 lightDir = WorldPosition - spotLight[i].position;
	    vec3 DSA = BlinnPhongComponents(normal, viewVector, lightDir, spotLight[i].color, spotLight[i].ambient);
	    float lightEffect = CalculateLightSpot(spotLight[i], WorldPosition);
	    
	    color += DSA * lightEffect;
	}

	FragmentColor = texture(diffuseTexture, TexCoord) * vec4(color, material.diffuse.w);
}



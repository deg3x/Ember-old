#version 330 core

in vec3 Normal;
in vec3 WorldPos;

out vec4 FragColor;

uniform vec4 color;
uniform vec4 lightColor;
uniform vec3 lightPos;
uniform vec3 cameraPos;

void main()
{
	float ambientLightStrength = 0.05f;
	vec3 ambient = vec3(ambientLightStrength * lightColor);

	vec3 lightDir = normalize(lightPos - WorldPos);
	vec3 normal = normalize(Normal);
	
	float diffuseContribution = max(dot(normal, lightDir), 0.0f);
	vec3 diffuse = vec3(diffuseContribution * lightColor);

	vec3 viewDir = normalize(cameraPos - WorldPos);
	vec3 reflectDir = reflect(-lightDir, normal);

	float specularStrength = 0.9f;
	float specularContribution = pow(max(dot(viewDir, reflectDir), 0.0f), 32);
	vec3 specular = specularStrength * specularContribution * vec3(lightColor);

	vec3 result = vec3(color) * (ambient + diffuse + specular);

	FragColor = vec4(result, 1.0f);
}
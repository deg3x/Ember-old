#version 330 core

in vec3 Normal;
in vec3 WorldPos;

out vec4 FragColor;

struct Material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess
};

struct Light
{
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform Material material;
uniform Light light;
uniform vec4 lightColor;
uniform vec3 lightPos;
uniform vec3 cameraPos;

void main()
{
	vec3 ambient = light.ambient * lightColor * material.ambient;

	vec3 lightDir = normalize(lightPos - WorldPos);
	vec3 normal = normalize(Normal);
	
	float diffuseContribution = max(dot(normal, lightDir), 0.0f);
	vec3 diffuse = light.diffuse * (diffuseContribution * material.diffuse);

	vec3 viewDir = normalize(cameraPos - WorldPos);
	vec3 reflectDir = reflect(-lightDir, normal);

	float specularContribution = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
	vec3 specular = light.specular * specularContribution * material.specular;

	vec3 result = ambient + diffuse + specular;

	FragColor = vec4(result, 1.0f);
}
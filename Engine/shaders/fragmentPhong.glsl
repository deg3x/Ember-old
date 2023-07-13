#version 410 core

in vec3 Normal;
in vec2 TexCoord;
in vec3 WorldPosition;

out vec4 FragmentColor;

struct Material
{
	vec4 diffuse;
	vec3 specular;
	float shininess;
};

struct DirectionalLight
{
	vec3 ambient;
	vec3 diffuse;
	vec3 direction;
	float intensity;
};

struct PointLight
{
	vec3 ambient;
	vec3 diffuse;
	vec3 position;
	float intensity;
	float constantAttenuation;
	float linearAttenuation;
	float quadraticAttenuation;
};

struct SpotLight
{
	vec3 ambient;
	vec3 diffuse;
	vec3 position;
	vec3 direction;
	float intensity;
	float constantAttenuation;
	float linearAttenuation;
	float quadraticAttenuation;
	float cutOffAngleCos;
	float cutOffAngleOutCos;
};

uniform Material material;
uniform DirectionalLight directionalLight;
uniform PointLight pointLight;
uniform SpotLight spotLight;
uniform vec3 cameraPosition;
uniform sampler2D diffuseTexture;

vec3 CalculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDirection);
vec3 CalculatePointLight(PointLight light, vec3 normal, vec3 viewDirection);
vec3 CalculateSpotLight(SpotLight light, vec3 normal, vec3 viewDirection);

void main()
{
	vec3 normal = normalize(Normal);
	vec3 viewDirection = normalize(cameraPosition - WorldPosition);

	//vec3 color = CalculateSpotLight(spotLight, normal, viewDirection);
	vec3 color = CalculateDirectionalLight(directionalLight, normal, viewDirection);
	//color += CalculateDirectionalLight(directionalLight, normal, viewDirection);
	//color += CalculatePointLight(pointLight, normal, viewDirection);
	//color += CalculateSpotLight(spotLight, normal, viewDirection);

	FragmentColor = texture(diffuseTexture, TexCoord) * vec4(color, material.diffuse.w);
}

vec3 CalculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDirection)
{
	vec3 lightVector = normalize(-light.direction);

	float diff = max(dot(normal, lightVector), 0.0);

	vec3 reflectVector = reflect(-lightVector, normal);
	float spec = pow(max(dot(viewDirection, reflectVector), 0.0), material.shininess);

	vec3 ambient = light.ambient * material.diffuse.xyz;
	vec3 diffuse = light.diffuse * diff * material.diffuse.xyz;
	vec3 specular = light.diffuse * spec * material.specular;

	return light.intensity * (ambient + diffuse + specular);
}

vec3 CalculatePointLight(PointLight light, vec3 normal, vec3 viewDirection)
{
	vec3 lightVector = normalize(light.position - WorldPosition);

	float diff = max(dot(normal, lightVector), 0.0);

	vec3 reflectVector = reflect(-lightVector, normal);
	float spec = pow(max(dot(viewDirection, reflectVector), 0.0), material.shininess);

	float distance = length(light.position - WorldPosition);
	float attenuation = 1.0 / (light.constantAttenuation + light.linearAttenuation * distance 
		+ light.quadraticAttenuation * (distance * distance));

	vec3 ambient = light.ambient * material.diffuse.xyz;
	vec3 diffuse = light.diffuse * diff * material.diffuse.xyz;
	vec3 specular = light.diffuse * spec * material.specular;

	return light.intensity * attenuation * (ambient + diffuse + specular);
}

vec3 CalculateSpotLight(SpotLight light, vec3 normal, vec3 viewDirection)
{
	vec3 lightVector = normalize(light.position - WorldPosition);

	float diff = max(dot(normal, lightVector), 0.0);

	vec3 reflectVector = reflect(-lightVector, normal);
	float spec = pow(max(dot(viewDirection, reflectVector), 0.0), material.shininess);

	float distance = length(light.position - WorldPosition);
	float attenuation = 1.0 / (light.constantAttenuation + light.linearAttenuation * distance 
		+ light.quadraticAttenuation * (distance * distance));

	float theta = dot(lightVector, normalize(-light.direction));
	float epsilon = light.cutOffAngleCos - light.cutOffAngleOutCos;
	float intensity = clamp((theta - light.cutOffAngleOutCos) / epsilon, 0.0, 1.0);

	vec3 ambient = light.ambient * material.diffuse.xyz;
	vec3 diffuse = light.diffuse * diff * material.diffuse.xyz, TexCoords;
	vec3 specular = light.diffuse * spec * material.specular;

	return light.intensity * attenuation * intensity * (ambient + diffuse + specular);
}

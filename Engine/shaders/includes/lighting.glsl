struct DirectionalLight
{
	vec3 color;
	vec3 ambient;
	vec3 direction;
	float intensity;
};

struct PointLight
{
	vec3 color;
	vec3 ambient;
	vec3 position;
	float intensity;
	float constantAttenuation;
	float linearAttenuation;
	float quadraticAttenuation;
};

struct SpotLight
{
	vec3 color;
	vec3 ambient;
	vec3 position;
	vec3 direction;
	float intensity;
	float constantAttenuation;
	float linearAttenuation;
	float quadraticAttenuation;
	float cutOffAngleCos;
	float cutOffAngleOutCos;
};

#define MAX_DIR_LIGHTS 4
#define MAX_POINT_LIGHTS 32
#define MAX_SPOT_LIGHTS 32
uniform DirectionalLight dirLights[MAX_DIR_LIGHTS];
uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform SpotLight spotLights[MAX_SPOT_LIGHTS];
uniform int activeLightsDir;
uniform int activeLightsPoint;
uniform int activeLightsSpot;

float CalculateLightDirectional(DirectionalLight light)
{
    return light.intensity;
}

float CalculateLightPoint(PointLight light, vec3 worldPos)
{
    float distance = length(light.position - worldPos);
	float attenuation = 1.0 / (light.constantAttenuation + light.linearAttenuation * distance + light.quadraticAttenuation * (distance * distance));
	
	return light.intensity * attenuation;
}

float CalculateLightSpot(SpotLight light, vec3 worldPos)
{
    float distance = length(light.position - worldPos);
	float attenuation = 1.0 / (light.constantAttenuation + light.linearAttenuation * distance + light.quadraticAttenuation * (distance * distance));
	
	vec3 lightVector = normalize(light.position - worldPos);
	
	float theta           = dot(lightVector, normalize(-light.direction));
	float epsilon         = light.cutOffAngleCos - light.cutOffAngleOutCos;
	float intensityCutoff = clamp((theta - light.cutOffAngleOutCos) / epsilon, 0.0, 1.0);
		
    return light.intensity * attenuation * intensityCutoff;
}
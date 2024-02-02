struct Material
{
	vec4 diffuse;
	vec3 specular;
	float shininess;
};

uniform Material material;

vec3 BlinnPhongComponents(vec3 normal, vec3 viewVector, vec3 lightDir, vec3 lightColor, vec3 lightAmbient)
{
    vec3 lightVector = normalize(-lightDir);

	float diff = max(dot(normal, lightVector), 0.0);

    vec3 halfVector = normalize(lightVector + viewVector);
	float spec = pow(max(dot(normal, halfVector), 0.0), material.shininess);

	vec3 ambient  = lightAmbient * material.diffuse.xyz;
	vec3 diffuse  = lightColor * diff * material.diffuse.xyz;
	vec3 specular = lightColor * spec * material.specular;
	
	return ambient + diffuse + specular;
}
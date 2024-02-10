#version 410 core

in vec3 TexCoords;

out vec4 FragmentColor;

uniform samplerCube skybox;

void main()
{
    vec3 skyboxColor = texture(skybox, TexCoords).rgb;
    
    skyboxColor = skyboxColor / (skyboxColor + vec3(1.0));
    skyboxColor = pow(skyboxColor, vec3(1.0/2.2));
    
    FragmentColor = vec4(skyboxColor, 1.0);
}

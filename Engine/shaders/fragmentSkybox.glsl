#version 410 core

in vec3 TexCoords;

out vec4 FragmentColor;

uniform samplerCube skybox;

void main()
{
    FragmentColor = texture(skybox, TexCoords);
}
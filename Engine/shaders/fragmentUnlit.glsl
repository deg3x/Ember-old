#version 410 core

in vec2 TexCoord;

out vec4 FragColor;

uniform vec4 color;
uniform sampler2D diffuseTexture;

void main()
{
	FragColor = texture(diffuseTexture, TexCoord) * color;
}

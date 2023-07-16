#version 330 core
in vec2 TextCoords;

uniform sampler2D texture_diffuse;

out vec4 FragColor;

void main()
{
	FragColor = texture(texture_diffuse, TextCoords);
}

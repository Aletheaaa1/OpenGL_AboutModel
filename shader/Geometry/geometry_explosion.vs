#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 2) in vec2 aTexture;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out VS_OUT{
	vec2 TextCoords;
} vs_out;

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0);
	vs_out.TextCoords = aTexture;
}
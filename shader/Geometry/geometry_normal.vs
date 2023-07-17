#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 model;


out VS_OUT{
	vec3 Normal;
	vec3 Pos;
} vs_out;

void main()
{
	vs_out.Normal =normalize( mat3(transpose(inverse(model))) * aNormal); 
	vs_out.Pos = aPos;
	gl_Position = model * vec4(aPos, 1.0);
}


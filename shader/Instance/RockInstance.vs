#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexture;

layout( location = 3) in mat4 aInstanceMat;

uniform mat4 view;
uniform mat4 projection;

out vec3 FragPosition;
out vec3 Normal;
out vec2 TextCoords;
void main()
{
	Normal = normalize(mat3(transpose(inverse(aInstanceMat))) * aNormal);
    FragPosition = vec3(aInstanceMat * vec4(aPos, 1.0));
	TextCoords = aTexture;
	gl_Position = projection * view * aInstanceMat *  vec4 (aPos, 1.0);
}


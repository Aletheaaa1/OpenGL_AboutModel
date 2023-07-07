#shader vertex 
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 aUVCoord;

uniform mat4 moduleMat;
uniform mat4 viewMat;
uniform mat4 projMat;

out vec3 Normal;
out vec3 FragPos;
out vec2 UVCoord;

void main()
{
    gl_Position = projMat * viewMat * moduleMat * vec4(aPos, 1.0);
    FragPos = (moduleMat * vec4(aPos.xyz, 1.0)).xyz;
    Normal = normalize(mat3(transpose(inverse(moduleMat))) * normal);    
    UVCoord = aUVCoord;
}
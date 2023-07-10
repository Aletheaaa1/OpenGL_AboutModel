#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexTCoords;

out vec2 TextCoords;
out vec3 Normal;
out vec3 FragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    Normal = normalize(mat3(transpose(inverse(model))) * aNormal);
    FragPos =vec3( model * vec4(aPos.xyz, 1.0) );

    TextCoords = aTexTCoords;    
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}
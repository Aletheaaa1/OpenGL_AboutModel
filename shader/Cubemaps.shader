#shader vertex
#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;

uniform mat4 module;
uniform mat4 view;
uniform mat4 projection;

out vec3 Normal;
out vec3 fragPos;

void main()
{
    Normal = mat3(transpose(inverse(module))) * aNormal;
    fragPos = vec3(module * vec4(aPos, 1.0));
    gl_Position = projection * view * module * vec4(aPos, 1.0);   
}

#shader fragment
#version 330 core

in vec3 Normal;
in vec3 fragPos;

uniform samplerCube skybox;
uniform vec3 cameraPos;

out vec4 color;
void main()
{
    vec3 I = normalize(fragPos - cameraPos);
    // ∑¥…‰
    vec3 R = reflect(I, normalize(Normal));
    // ’€…‰
    // vec3 R = refract(I, normalize(Normal), 0.658);
    color = vec4(texture(skybox, R).rgb, 1.0);   
}
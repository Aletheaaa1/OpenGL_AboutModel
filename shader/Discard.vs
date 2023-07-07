
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexture;

uniform mat4 module;
uniform mat4 view;
uniform mat4 projection;

out vec2 Texture;
void main()
{
    gl_Position = projection * view * module * vec4(aPos, 1.0);   
    Texture = aTexture;
}


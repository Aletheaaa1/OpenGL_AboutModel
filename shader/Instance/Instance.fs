#version 330 core
in vec3 Color;
uniform sampler2D Inside;
uniform sampler2D OutSide;
out vec4 color;
void main()
{
    color = vec4(Color, 1.0);
}
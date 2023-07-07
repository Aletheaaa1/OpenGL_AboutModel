#shader vertex
#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aOffset;
 
out vec3 Color;
void main()
{
    Color = aColor;
    vec2 pos = aPos * (gl_InstanceID / 100.0);
    gl_Position = vec4(pos + aOffset, 0, 1.0);  
    // gl_Position = vec4(aPos, 1.0f, 1.0f);  
}

#shader fragment
#version 330 core
in vec3 Color;
uniform sampler2D Inside;
uniform sampler2D OutSide;
out vec4 color;
void main()
{
    color = vec4(Color, 1.0);
}
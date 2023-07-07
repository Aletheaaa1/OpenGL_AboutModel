#shader vertex
#version 330 core
layout (location = 0) in vec3 aPos;
uniform mat4 module;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * module * vec4(aPos, 1.0);    
}

#shader fragment
#version 330 core
uniform sampler2D Inside;
uniform sampler2D OutSide;
out vec4 color;
void main()
{
    if(gl_FrontFacing)
        color = vec4(1.0, 0.0, 1.0, 0.0);   
    else
        color = vec4(0.0, 1.0, 0.0, 1.0);
}
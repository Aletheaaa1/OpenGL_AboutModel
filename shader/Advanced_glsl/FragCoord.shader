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
out vec4 color;
void main()
{
    if(sin(gl_FragCoord.x) > 0.3)
        color = vec4(1.0, 0.0, 1.0, 0.0);   
    else
        color = vec4(0.0, 1.0, 0.0, 1.0);
}
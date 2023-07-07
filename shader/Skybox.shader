#shader vertex
#version 330 core

layout(location = 0) in vec3 aPos;

uniform mat4 view;
uniform mat4 projection;

out vec3 textCoord;
void main()
{
    textCoord = aPos;
    vec4 pos = projection * view * vec4(aPos, 1.0);
    gl_Position = pos.xyww;
}

#shader fragment
#version 330 core

in vec3 textCoord;

uniform samplerCube skybox;

out vec4 color;

void main()
{
    color = texture(skybox, textCoord);   
}
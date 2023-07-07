#shader vertex
#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTextCoord;

out vec2 TextCoord;
void main()
{
    TextCoord = aTextCoord;
    gl_Position = vec4(aPos, 0.0, 1.0);   
}

#shader fragment
#version 330 core
in vec2 TextCoord;
uniform sampler2D screenTexture;
out vec4 color;
void main()
{
    color = vec4(texture(screenTexture, TextCoord).rgb, 1.0);   
}
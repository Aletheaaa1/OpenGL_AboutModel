
#version 330 core
in vec2 Texture;

uniform sampler2D tex;

out vec4 color;
void main()
{
    vec4 FragColor = texture(tex, Texture);   
    if(FragColor.a < 0.1)
    {
        discard;
    }
    color = FragColor;
}
#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture_diffuse1;

out vec4 color;
void main()
{    
   FragColor = texture(texture_diffuse1, TexCoords);
   // color = vec4(1.0, 1.0, 1.0, 1.0);
}
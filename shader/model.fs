#version 330 core
in vec2 TexCoords;
out vec4 FragColor;
uniform sampler2D texture_diffuse;

out vec4 color;
void main()
{    
   FragColor = texture(texture_diffuse, TexCoords);
   // color = vec4(1.0, 1.0, 1.0, 1.0);
}
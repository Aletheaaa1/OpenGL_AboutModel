#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 Position;
in vec2 TexCoords;

uniform sampler2D texture_reflection;
uniform sampler2D texture_diffuse;
uniform sampler2D texture_specular;
uniform sampler2D texture_normal;
uniform vec3 CameraPosition;
uniform samplerCube Skybox;

void main()
{    
	vec3 I = normalize(Position - CameraPosition);
	vec3 R = reflect(I, normalize(Normal));

	vec3 reflection = vec3(texture(texture_reflection, TexCoords));
	reflection = reflection * texture(Skybox, R).rgb;

	float diffuse_rate = max(dot(Normal, I), 0); 
	vec3 diffuse = diffuse_rate * vec3(texture(texture_diffuse, TexCoords));

	vec3 result = vec3(reflection + diffuse);
	FragColor = vec4(result, 1.0);
}
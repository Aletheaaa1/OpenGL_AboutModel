#version 330 core
in vec2 TextCoords;
in vec3 Normal;
in vec3 FragPosition;

uniform sampler2D texture_diffuse;
uniform vec3 LightDir;
uniform vec3 CameraPos;

out vec4 FragColor;
void main()
{
	vec3 CameraVec = normalize(CameraPos - FragPosition);
	vec3 R = normalize(reflect(-LightDir, Normal));
	vec3 HalfVec = normalize(LightDir + CameraVec);
	
	float SpecularRationBlinn = pow(max(dot(Normal, HalfVec), 0.0), 32);
	float SpecularRationPhone = pow(max(dot(CameraVec, R), 0.0), 32);
	vec4 Specular = texture(texture_diffuse, TextCoords) * SpecularRationBlinn;

	float DiffuseRation = max(dot(LightDir, Normal), 0.0);
	vec4 Diffuse =  texture(texture_diffuse, TextCoords) * DiffuseRation ;

	FragColor = ( Diffuse + Specular );
}

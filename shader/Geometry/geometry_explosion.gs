#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in VS_OUT{
	vec2 TextCoords;
}gs_in[];

uniform float time;

out vec2 TextCoords;

vec4 Explosion(vec4 position, vec3 normal)
{
	float offset = 2.0;
	vec3 direction = normal * ((sin(time) + 1.0) / 2.0) * offset;
	return position + vec4(direction, 1.0);
}

vec3 GetNormal()
{
	vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
	vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);
	return normalize(cross(a , b));
}

void main()
{
	vec3 normal = GetNormal();

	gl_Position = Explosion(gl_in[0].gl_Position, normal);
	//TextCoords = (gs_in[0].TextCoords + gs_in[1].TextCoords +gs_in[2].TextCoords) / 3.0;
	TextCoords = gs_in[0].TextCoords;
	EmitVertex();
	
	gl_Position = Explosion(gl_in[1].gl_Position, normal);
	TextCoords = gs_in[1].TextCoords;
	EmitVertex();

	gl_Position = Explosion(gl_in[2].gl_Position, normal);
	TextCoords = gs_in[2].TextCoords;
	EmitVertex();
	
	EndPrimitive();	
}

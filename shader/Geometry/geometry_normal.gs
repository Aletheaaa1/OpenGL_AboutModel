#version 330 core
layout (triangles) in;
layout (line_strip, max_vertices = 6) out;

in VS_OUT{
	vec3 Normal;
	vec3 Pos;
} gs_in[];

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

const float RATION = 1;

void DrawLine(int index)
{
	mat4 mvp = projection * view ;

	gl_Position =  mvp * vec4(gs_in[index].Pos, 1.0);
	EmitVertex();
	gl_Position =  mvp * vec4(gs_in[index].Pos, 1.0) + mvp * RATION * vec4(gs_in[index].Normal, 0.0);
	EmitVertex();

	EndPrimitive();
}

void main()
{
	DrawLine(0);
	DrawLine(1);
	DrawLine(2);
}


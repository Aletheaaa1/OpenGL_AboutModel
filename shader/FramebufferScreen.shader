#shader vertex
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 TexCoords;

void main()
{
	TexCoords = aTexCoords;
	gl_Position = vec4(aPos.xy, 0.0, 1.0);
}

#shader fragment
#version 330 core

in vec2 TexCoords;

uniform sampler2D screenTexture;

out vec4 color;
void main()
{
	const float offset = 1.0 / 300.0;
	vec2 offsets[9] = vec2[](
		vec2(-offset, offset), vec2(0.0, offset), vec2(offset, offset),
		vec2(-offset, 0.0), vec2(0.0, 0.0), vec2(offset, 0.0),
		vec2(-offset, -offset), vec2(0.0, -offset), vec2(offset, -offset)
	);

	float kernel[9] = float[](
		1, 1, 1,
        1, -8, 1,
        1, 1, 1
	);
	vec3 sampleTex[9];
    for(int i = 0; i < 9; i++)
    {
        sampleTex[i] = vec3(texture(screenTexture, TexCoords.st + offsets[i]));
    }
    vec3 col = vec3(0.0);
    for(int i = 0; i < 9; i++)
        col += sampleTex[i] * kernel[i];

	// color = vec4(col, 1.0);
	color = vec4(vec3(texture(screenTexture, TexCoords)), 1.0);
}
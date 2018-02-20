#version 140
in vec2 UVs;

uniform sampler2D tex;

out vec4 color;

void main()
{
	color = texture(tex,UVs);
}
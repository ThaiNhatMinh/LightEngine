#version 140
in vec2 UVs;

uniform mat4 MVP;
uniform sampler2D tex;

out vec4 color;

void main()
{
	color = texture(tex,Uvs);
}
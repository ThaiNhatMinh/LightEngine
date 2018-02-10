#version 140
in vec2 pos;
in vec2 uv;

uniform mat4 MVP;

out vec2 UVs;

void main()
{
	gl_Position = MVP * vec4(pos, 0,1.0f);
	UVs = uv;
}
#version 140
in vec4 pos;

uniform mat4 MVP;

out vec2 UVs;

void main()
{
	gl_Position = MVP * vec4(pos.xy, 0.0,1.0f);
	UVs = pos.zw;
}
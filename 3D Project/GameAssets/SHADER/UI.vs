#version 140
in vec4 pos;

uniform mat4 MVP;
uniform vec2 Translate;
out vec2 UVs;

void main()
{
	vec2 finalPos = vec2(Translate.x + pos.x,Translate.y + pos.y);
	
	gl_Position = MVP * vec4(finalPos, 0.0,1.0f);
	UVs = pos.zw;
}
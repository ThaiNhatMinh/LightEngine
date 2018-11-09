#version 140
in vec2 aPos;

uniform mat4 uMVP;
uniform vec2 uPos;
uniform vec2 uSize;

out vec2 UVs;

void main()
{
	vec2 pos = aPos;
	if(gl_VertexID==1) pos.x = uSize.x;
	else if(gl_VertexID==2) pos.y = uSize.y;
	else if(gl_VertexID==3) pos = uSize;
	vec2 finalPos = vec2(uPos.x + pos.x,uPos.y + pos.y);
	
	gl_Position = uMVP * vec4(finalPos, 0.0,1.0f);
	UVs = vec2(aPos.y,-aPos.x);
}
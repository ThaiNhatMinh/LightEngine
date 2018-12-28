#version 330 core
in vec2 aPos;

uniform mat4 uMVP;
uniform vec2 uPos;
uniform vec2 uSize;

out vec2 UVs;

void main()
{
	vec2 pos = uPos;
	if(gl_VertexID==1) pos.x += uSize.x;
	else if(gl_VertexID==2) pos.y += uSize.y;
	else if(gl_VertexID==3) pos += uSize;
	gl_Position = uMVP * vec4(pos, 0.0,1.0f);
	UVs = vec2(aPos.y,1-aPos.x);
}
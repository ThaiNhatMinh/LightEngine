#version 140
in vec3 pos;

uniform mat4 MVP;

out vec3 uv;

void main()
{
	vec4 posL = MVP* vec4(pos,1.0f);
	gl_Position = posL.xyww;
	uv = vec3(pos.x,-pos.y,pos.z);
}
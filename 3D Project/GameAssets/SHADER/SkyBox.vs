#version 140
in vec3 pos;

uniform mat4 MVP;

out vec3 uv;

void main()
{
	gl_Position = MVP* vec4(pos,1.0f);
	uv = vec3(pos.x,-pos.y,pos.z);
}
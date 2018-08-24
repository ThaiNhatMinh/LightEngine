#version 140
in vec3 uv;

uniform samplerCube Tex;

out vec4 color;

void main()
{
	color = texture(Tex,uv);
	//color = vec4(1.0,0.5,0.2,1.0);
}
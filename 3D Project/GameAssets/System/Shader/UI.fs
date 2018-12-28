#version 330 core
in vec2 UVs;

uniform sampler2D tex;
uniform vec3 objColor;
out vec4 color;
uniform bool isText = false;
void main()
{
	if(isText)
	{
		vec4 sampled = vec4(1.0, 1.0, 1.0, texture(tex, UVs).r);
		color = vec4(1,1,1,1.0)*sampled;
	}
	else color = texture(tex, UVs);
	//color = vec4(0.5,0.6,0.4,0.6);
}
#version 140
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
}
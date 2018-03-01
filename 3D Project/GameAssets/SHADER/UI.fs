#version 140
in vec2 UVs;

uniform sampler2D tex;
uniform vec3 objColor;
out vec4 color;

void main()
{
	vec4 sampled = vec4(1.0, 1.0, 1.0, texture(tex, UVs).r);
    color = vec4(objColor,1.0)*sampled;
}
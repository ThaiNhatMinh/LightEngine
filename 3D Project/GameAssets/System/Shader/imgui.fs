#version 140

uniform sampler2D uTex;

in vec2 oUV;
in vec4 oColor;

out vec4 Out_Color;

void main()
{
	Out_Color = oColor * texture( uTex, oUV.st);
}
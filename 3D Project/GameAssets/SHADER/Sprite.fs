#version 140

in vec2 vUV;
out vec4 color;
uniform sampler2D Tex;


void main()
{
	color = texture(Tex,vUV);

}
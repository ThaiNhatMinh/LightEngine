#version 140


in vec3 vNormal;
in vec2 vUV;
in vec3 vFragPos;

uniform sampler2D Tex;

out vec4 pixel;

void main()
{
	pixel = texture(Tex,vUV);
}
#version 140
#pragma optimize (off)
in vec2 oUV;
in vec3 oNormal;

out vec4 Color;


void main()
{
    Color = vec4(oNormal, 1.0f);
}
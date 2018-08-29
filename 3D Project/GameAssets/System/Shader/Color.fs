#version 140

uniform vec3 uColor;

out vec3 color;

void main()
{
    color = vec4(uColor,1.0f);
}
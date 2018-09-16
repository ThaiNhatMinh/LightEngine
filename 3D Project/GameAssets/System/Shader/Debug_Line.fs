#version 140

in vec3 uColor;

out vec4 color;

void main()
{
    color = vec4(uColor,1.0f);
}
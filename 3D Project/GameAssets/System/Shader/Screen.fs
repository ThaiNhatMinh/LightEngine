#version 140

in vec2 oUV;

out vec4 oColor;

uniform sampler2D uTex;

void main()
{
    oColor = texture(uTex,oUV);
}


#version 140

uniform mat4 uMVP;

in vec2 aPos;
in vec2 aUV;
in vec4 aColor;

out vec2 oUV;
out vec4 oColor;

void main()
{
   	oUV = aUV;
   	oColor = aColor;
   	gl_Position = uMVP * vec4(aPos.xy,0,1);
}
#version 140
#pragma optimize (off)
in vec2 oUV;
in vec3 oNormal;

uniform sampler2D uTex;
out vec4 Color;


void main()
{
    //vec3 a = oNormal;
   // Color *= vec4(a, 1.0f);
    Color = texture(uTex,vec2(oUV.x,oUV.y));
    //Color = vec4(oNormal,1.0f);
}
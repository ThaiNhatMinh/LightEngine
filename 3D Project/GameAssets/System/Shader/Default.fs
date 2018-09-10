#version 140
#pragma optimize (off)
in vec2 oUV;
in vec3 oNormal;
in vec3 oPosition;

uniform sampler2D uTex;
uniform samplerCube uCubeTex;
uniform vec3 uCameraPos;

out vec4 Color;


void main()
{
    vec3 I = normalize(oPosition - uCameraPos);
    vec3 R = reflect(I,normalize(oNormal));
    R.y = -R.y;
    
    Color = texture(uTex,vec2(oUV.x,oUV.y));
    Color = vec4(texture(uCubeTex, R).rgb, 1.0);
   
}
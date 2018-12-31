#version 330 core
in vec3 aPos;
in vec3 aNormal;
in vec2 aUV;


out VS_OUT
{
    vec2 oUV;
    vec2 oUVScale;
    vec3 oNormal;
    vec3 oPosition;
}vs_out;

uniform mat4 uMVP;
uniform mat4 uModel;

void main()
{
    vs_out.oNormal =  mat3(transpose(inverse(uModel))) * aNormal;  
	vs_out.oPosition = vec3(uModel * vec4(aPos,1.0f));

    float scale = 10;
	vs_out.oUV = aUV;
    vs_out.oUVScale = aUV*scale;
    gl_Position = uMVP * vec4(aPos, 1.0f);
}
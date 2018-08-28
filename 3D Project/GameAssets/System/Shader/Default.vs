#version 140
#pragma optimize (off)
in vec3 aPos;
in vec3 aNormal;
in vec2 aUV;

out vec2 oUV;
out vec3 oNormal;

uniform mat4 uMVP;
uniform mat4 uModel;

void main()
{
    oUV = aUV;
    oNormal = mat3(uModel) * aNormal;
    
    gl_Position = uMVP * vec4(aPos, 1.0f);
}
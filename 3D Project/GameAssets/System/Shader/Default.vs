#version 140

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
    oNormal = oNormal;
    
    gl_Position = uMVP * vec4(aPos, 1.0f);
}
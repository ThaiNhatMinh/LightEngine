#version 140

in vec3 aPos;
in vec3 aColor;

out vec3 uColor;
uniform mat4 uMVP;

void main()
{
    gl_Position = uMVP*vec4(aPos,1.0f);
    uColor = aColor;
}
#version 140

in vec3 aPos;
uniform mat4 uMVP;
uniform mat4 uModel;
void main()
{
    gl_Position = uMVP*uModel*vec4(aPos,1.0f);
}
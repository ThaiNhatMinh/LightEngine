#version 140
in vec2 aPos;
in vec2 aUV;

out vec2 oUV;

void main()
{
    gl_Position = vec4(aPos.x, aPos.y,0.0f, 1.0f);
    oUV = aUV;
}

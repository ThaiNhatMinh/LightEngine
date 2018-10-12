#version 140
in vec3 aPos;

uniform mat4 uMVP;

out vec3 oUV;

void main()
{
	vec4 posL = uMVP* vec4(aPos,1.0f);
	gl_Position = posL.xyww;
	oUV = vec3(aPos.x,-aPos.y,aPos.z);
}
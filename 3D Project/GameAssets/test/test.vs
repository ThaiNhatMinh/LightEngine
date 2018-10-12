#version 140
in vec3 aPos;
in vec2 aUV;
out vec2 oUV;
void main(){
oUV = aUV;
gl_Position = vec4(aPos,1.0);
};
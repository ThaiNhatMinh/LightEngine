#version 140
in vec2 oUV;
out vec3 color;
uniform sampler2D uTex;
void main(){
 color = vec3(texture(uTex,oUV));
};
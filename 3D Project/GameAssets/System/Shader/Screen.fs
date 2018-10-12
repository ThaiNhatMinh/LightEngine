#version 140

in vec2 oUV;

out vec4 oColor;

uniform sampler2D uTex;
uniform sampler2D uDepthTex;
const float offset = 1.0 / 300.0;  

float zfar = 5000.0f;
float znear = 0.1f;

float linearize(float depth)
{
    return (-zfar * znear / (depth * (zfar - znear) - zfar)) / zfar;
}

void main()
{
    vec2 offsets[9] = vec2[](
        vec2(-offset,  offset), // top-left
        vec2( 0.0f,    offset), // top-center
        vec2( offset,  offset), // top-right
        vec2(-offset,  0.0f),   // center-left
        vec2( 0.0f,    0.0f),   // center-center
        vec2( offset,  0.0f),   // center-right
        vec2(-offset, -offset), // bottom-left
        vec2( 0.0f,   -offset), // bottom-center
        vec2( offset, -offset)  // bottom-right    
    );

    float kernel[9] = float[](
    1.0 / 16, 2.0 / 16, 1.0 / 16,
    2.0 / 16, 4.0 / 16, 2.0 / 16,
    1.0 / 16, 2.0 / 16, 1.0 / 16  
    );
    
    vec3 sampleTex[9];
    for(int i = 0; i < 9; i++)
    {
        sampleTex[i] = vec3(texture(uTex, oUV.st + offsets[i]));
    }
    vec3 col = vec3(0.0);
    for(int i = 0; i < 9; i++)
        col += sampleTex[i] * kernel[i];
    

    float depth = linearize(texture(uDepthTex,oUV).r);
    if(depth<=0.02f) oColor = vec4(col, 1.0);
    else oColor = texture(uTex, oUV);
    //oColor = vec4(vec3(linearize(depth)),1.0f);
    //oColor = texture(uTex,oUV);
    
}  

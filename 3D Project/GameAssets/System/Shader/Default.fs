#version 330
#define MAX_POINTLIGHT 10
struct DirLight
{
    vec3 Ia;
    vec3 Id;
    vec3 Is;
    vec3 Direction;
};
struct PointLight
{
    vec3 Ia;
    vec3 Id;
    vec3 Is;
    vec3 Pos;
    float constant;
    float linear;
    float quadratic;
};

struct Material
{
    //sampler2D ambient;
    sampler2D diffuse;
    //sampler2D specular;
    vec3 Ka;
    vec3 Kd;
    vec3 Ks;
    float shininess;
};

in VS_OUT
{
    vec2 oUV;
    vec3 oNormal;
    vec3 oPosition;
}fs_in;


uniform Material mat;
uniform samplerCube uCubeTex;
uniform vec3 uCameraPos;
uniform DirLight dLight;
uniform PointLight pLights[MAX_POINTLIGHT];
uniform int uNumLight;
out vec4 Color;

struct SurfaceInfo
{
    vec4 texel;
    vec3 normal;
    vec3 position;
    vec3 viewDir;
};
vec4 ComputerDirLight(SurfaceInfo info)
{
    
    vec3 LightDir = normalize(-dLight.Direction);
    vec3 reflectDir = reflect(-LightDir,info.normal);

    float diff = max(dot(LightDir, info.normal),0.0f);
    float spec = pow(max(dot(info.viewDir, reflectDir),0.0f), 32);

    vec4 ambient = vec4(dLight.Ia,1.0f)* vec4(mat.Ka,1.0f) * info.texel;
    vec4 diffuse = vec4(dLight.Id,1.0f)* vec4(mat.Kd,1.0f) *diff * info.texel;
    vec4 specular = vec4(dLight.Is,1.0f)* vec4(mat.Ks,1.0f) *spec * info.texel;

    vec4 output = vec4(ambient + diffuse + specular);

    return output;
}

vec4 ComputerPointLight(SurfaceInfo info,PointLight pLight)
{
     vec3 LightDir = normalize(pLight.Pos - info.position);
    vec3 reflectDir = reflect(-LightDir,info.normal);

    float diff = max(dot(LightDir, info.normal),0.0f);
    float spec = pow(max(dot(info.viewDir, reflectDir),0.0f), mat.shininess);

    vec4 ambient =  vec4(pLight.Ia,1.0f) * vec4(mat.Ka,1.0f) *  info.texel;
    vec4 diffuse =  vec4(pLight.Id,1.0f) * vec4(mat.Kd,1.0f) * diff * info.texel;
    vec4 specular = vec4(pLight.Is,1.0f) * vec4(mat.Ks,1.0f) * spec * info.texel;

    // attenuation
    float distance    = length(pLight.Pos -  info.position);
    float attenuation = 1.0 / (pLight.constant + pLight.linear * distance + pLight.quadratic * (distance * distance));

    vec4 output = vec4(ambient + diffuse + specular);
    output *= attenuation;

    return output;
}
void main()
{
    SurfaceInfo infoo;
    infoo.normal = normalize(fs_in.oNormal);
    infoo.texel = texture(mat.diffuse,fs_in.oUV);
    infoo.position = fs_in.oPosition;
    infoo.viewDir = normalize(uCameraPos - fs_in.oPosition);

    Color = ComputerDirLight(infoo);
    for(int i=0; i<uNumLight; i++ )
        Color +=ComputerPointLight(infoo,pLights[i]);

}
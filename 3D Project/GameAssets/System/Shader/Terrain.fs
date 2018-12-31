#version 330 core
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
    sampler2D tex1;
    sampler2D tex2;
    sampler2D tex3;
    sampler2D blend;
    vec3 Ka;
    vec3 Kd;
    vec3 Ks;
    float shininess;
};

in VS_OUT
{
    vec2 oUV;
    vec2 oUVScale;
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

    vec4 ambient = vec4(dLight.Ia,1.0f) * vec4(mat.Ka,1.0f) * info.texel;
    vec4 diffuse = vec4(dLight.Id,1.0f)  * vec4(mat.Kd,1.0f)*diff * info.texel;
    vec4 specular = vec4(dLight.Is,1.0f) * vec4(mat.Ks,1.0f) *spec * info.texel;

    vec4 r = vec4(ambient + diffuse + specular);

    return r;
}

vec4 ComputerPointLight(SurfaceInfo info,int i)
{
     vec3 LightDir = normalize(pLights[i].Pos - info.position);
    vec3 reflectDir = reflect(-LightDir,info.normal);

    float diff = max(dot(LightDir, info.normal),0.0f);
    float spec = pow(max(dot(info.viewDir, reflectDir),0.0f), mat.shininess);

    vec4 ambient =  vec4(pLights[i].Ia,1.0f) * vec4(mat.Ka,1.0f) *  info.texel;
    vec4 diffuse =  vec4(pLights[i].Id,1.0f) * vec4(mat.Kd,1.0f) * diff * info.texel;
    vec4 specular = vec4(pLights[i].Is,1.0f) * vec4(mat.Ks,1.0f) * spec * info.texel;

    // attenuation
    float distance    = length(pLights[i].Pos -  info.position);
    float attenuation = 1.0 / (pLights[i].constant + pLights[i].linear * distance + pLights[i].quadratic * (distance * distance));

    vec4 r = vec4(ambient + diffuse + specular);
    r *= attenuation;

    return r;
}

vec4 BlendTexture()
{
    vec4 t1 = texture(mat.tex1,fs_in.oUVScale);
    vec4 t2 = texture(mat.tex2,fs_in.oUVScale);
    vec4 t3 = texture(mat.tex3,fs_in.oUVScale);
    vec4 blend = texture(mat.blend,fs_in.oUV);
    return t1;
    return mix(t1,mix(t2,t3,blend.g),blend.r);
 

}
void main()
{
    SurfaceInfo infoo;
    infoo.normal = normalize(fs_in.oNormal);
    infoo.texel = BlendTexture();
    infoo.position = fs_in.oPosition;
    infoo.viewDir = normalize(uCameraPos - fs_in.oPosition);
    infoo.texel = vec4(0.5f,1.0f,1.0f,1.0f);

    Color = ComputerDirLight(infoo);
    for(int i=0; i<uNumLight; i++ )
        Color +=ComputerPointLight(infoo,i);

    
}
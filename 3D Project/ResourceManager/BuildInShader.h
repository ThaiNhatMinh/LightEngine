#pragma once


static const char* Sprite_vs = R"(#version 130
in vec3 aPos;
out vec2 oUV;
uniform vec3 uSpritePos;
uniform vec3 uCameraRight;
uniform vec3 uCameraUp;
uniform vec2 uSpriteSize;
uniform mat4 uMVP;
void main()
{	
	vec3 vertexCenter = uSpritePos + uCameraRight*uSpriteSize.x * aPos.x + uCameraUp*uSpriteSize.y* aPos.y;
	gl_Position = uMVP * vec4(vertexCenter,1.0f);
	oUV = aPos.xy + vec2(0.5, 0.5);
})";

static const char* Sprite_fs = R"(#version 140
in vec2 oUV;
out vec4 color;
uniform sampler2D Tex;
void main()
{
	color = texture(Tex, oUV);
})";


static const char* DebugLine_vs = R"(#version 140
in vec3 aPos;
in vec3 aColor;
out vec3 uColor;
uniform mat4 uMVP;
void main()
{
    gl_Position = uMVP*vec4(aPos,1.0f);
    uColor = aColor;
})";


static const char* DebugLine_fs = R"(#version 140
in vec3 uColor;
out vec4 color;
void main()
{
    color = vec4(uColor,1.0f);
})";


static const char* DebugBox_vs = R"(#version 140
in vec3 aPos;
uniform mat4 uMVP;
uniform mat4 uModel;
void main()
{
    gl_Position = uMVP*uModel*vec4(aPos,1.0f);
})";


static const char* DebugBox_fs = R"(#version 140
uniform vec3 uColor;
out vec4 color;
void main()
{
    color = vec4(uColor,1.0f);
})";


static const char* UI_vs = R"(#version 330 core
in vec2 aPos;
uniform mat4 uMVP;
uniform vec2 uPos;
uniform vec2 uSize;
out vec2 UVs;
void main()
{
	vec2 pos = uPos;
	if(gl_VertexID==1) pos.x += uSize.x;
	else if(gl_VertexID==2) pos.y += uSize.y;
	else if(gl_VertexID==3) pos += uSize;
	gl_Position = uMVP * vec4(pos, 0.0,1.0f);
	UVs = vec2(aPos.y,1-aPos.x);
})";


static const char* UI_fs = R"(#version 330 core
in vec2 UVs;
uniform sampler2D tex;
uniform vec3 objColor;
out vec4 color;
uniform bool isText = false;
void main()
{
	if(isText)
	{
		vec4 sampled = vec4(1.0, 1.0, 1.0, texture(tex, UVs).r);
		color = vec4(1,1,1,1.0)*sampled;
	}
	else color = texture(tex, UVs);
	//color = vec4(0.5,0.6,0.4,0.6);
})";


static const char* Default_fs = R"(#version 330 core
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

    vec4 result = vec4(ambient + diffuse + specular);

    return result;
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

    vec4 result = vec4(ambient + diffuse + specular);
    result *= attenuation;

    return result;
}
vec4 temp(SurfaceInfo info)
{
    vec4 t = vec4(0,0,0,0);
    for(int i=0; i<uNumLight; i++ )
        t +=ComputerPointLight(info,i);
    return t;
}
void main()
{
    SurfaceInfo infoo;
    infoo.normal = normalize(fs_in.oNormal);
    infoo.texel = texture(mat.diffuse,fs_in.oUV);
    infoo.position = fs_in.oPosition;
    infoo.viewDir = normalize(uCameraPos - fs_in.oPosition);

    Color = ComputerDirLight(infoo);
    Color +=temp(infoo);
    
})";

static const char* Default_vs = R"(#version 330
in vec3 aPos;
in vec3 aNormal;
in vec2 aUV;


out VS_OUT
{
    vec2 oUV;
    vec3 oNormal;
    vec3 oPosition;
}vs_out;

uniform mat4 uMVP;
uniform mat4 uModel;

void main()
{
    vs_out.oNormal =  mat3(transpose(inverse(uModel))) * aNormal;  
	vs_out.oPosition = vec3(uModel * vec4(aPos,1.0f));
	vs_out.oUV = aUV;

    gl_Position = uMVP * vec4(aPos, 1.0f);
})";

static const char* Skeleton_vs = R"(#version 330
// neu co bien khong dung toi thi trinh bien dich se tu dong xoa khi bien dich.
in vec3 aPos;	// location = 0
in vec3 aNormal;		// location = 1
in vec2 aUV;			// location = 2, neu normal khong duoc dung toi no se bi xoa va location cua uv se la 1
in vec2 aBlend1;			// x: bone y:weight
in vec2 aBlend2;			// x: bone y:weight
in vec2 aBlend3;			// x: bone y:weight
in vec2 aBlend4;			// x: bone y:weight


uniform mat4 uMVP;
uniform mat4 uModel;
uniform mat4 uTransform[70];
uniform int uNumNode;
out VS_OUT
{
    vec2 oUV;
    vec3 oNormal;
    vec3 oPosition;
}vs_out;


void main()
{
	vec4 pos = vec4(aPos,1.0f);

	vec4 finalPos = vec4(0);
	vec3 finalNormal = vec3(0);
	int b;
	if((aBlend1.x<71 )&&( aBlend1.x>0))
	{
		b =int(aBlend1.x);
		finalPos += uTransform[b] * pos * vec4(aBlend1.y) ;
		finalNormal += mat3(uTransform[b])* aNormal * vec3(aBlend1.y);
	}

	if((aBlend2.x<71 )&& (aBlend2.x>0))
	{
		b = int(aBlend2.x);
		finalPos +=uTransform[b] * pos * vec4(aBlend2.y) ;
		finalNormal += mat3( uTransform[b])* aNormal * vec3(aBlend2.y);
	}

	if((aBlend3.x<71) && (aBlend3.x>0))
	{
		b =int(aBlend3.x);
		finalPos += uTransform[b] * pos * vec4(aBlend3.y) ;
		finalNormal += mat3(uTransform[b])* aNormal * vec3(aBlend3.y);
	}

	if((aBlend4.x<71) &&( aBlend4.x>0))
	{
		b =int(aBlend4.x);
		finalPos += uTransform[b] * (pos * vec4(aBlend4.y) );
		finalNormal +=   mat3(uTransform[b])* aNormal * vec3(aBlend4.y);
	}


	gl_Position = uMVP*finalPos;
	vs_out.oNormal =  mat3(transpose(inverse(uModel))) *finalNormal;  
	vs_out.oPosition = vec3(uModel * finalPos);
	vs_out.oUV = aUV;
	//oFragPos = vec3(uModel * pos);

	
})";
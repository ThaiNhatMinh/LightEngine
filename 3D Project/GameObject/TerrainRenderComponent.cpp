#include "pch.h"

#include <IL\il.h>
#include <IL\ilu.h>

const char* TerrainRenderComponent::Name = "TerrainRenderComponent";

bool TerrainRenderComponent::VInit(tinyxml2::XMLElement * pData)
{
	if (!pData) return false;
	tinyxml2::XMLElement* pModelPath = pData->FirstChildElement("Model");

	const char* pFileName = pModelPath->Attribute("File");
	if (pFileName)
	{
		Mesh* p = ReadFile(pFileName);
		p->Tex = gResources()->LoadTexture("GameAssets\\TEXTURE\\Default.png");
		m_Material.Ka = vec3(1.0f);
		m_Material.Kd = vec3(1.0f);
		m_Material.Ks = vec3(1.0f);
		m_Material.exp = 64;
		m_MeshList.push_back(p);
	}
	else if (pFileName = pModelPath->Attribute("Shape"))
	{
		m_MeshList.push_back(gResources()->CreateShape(SHAPE_BOX));
		m_Material.Ka = vec3(1.0f);
		m_Material.Kd = vec3(1.0f);
		m_Material.Ks = vec3(1.0f);
		m_Material.exp = 64;

		tinyxml2::XMLElement* pColor = pData->FirstChildElement("Color");
		for (size_t i = 0; i < m_MeshList.size(); i++)
		{
			m_MeshList[i]->Color = vec3(pColor->DoubleAttribute("r", 1.0f), pColor->DoubleAttribute("g", 1.0f), pColor->DoubleAttribute("b", 1.0f));
		}
	}

	tinyxml2::XMLElement* pScale = pData->FirstChildElement("Scale");
	if (pScale)
	{
		vec3 scale(pScale->DoubleAttribute("x", 1.0), pScale->DoubleAttribute("y", 1.0), pScale->DoubleAttribute("z", 1.0));

		for (size_t i = 0; i < m_MeshList.size(); i++)
		{
			m_MeshList[i]->Scale(scale);
		}
	}

	tinyxml2::XMLElement* pShader = pData->FirstChildElement("Shader");
	if (pShader)
	{
		m_pShader = gResources()->GetShader(pShader->Attribute("name"));
		if (m_pShader == nullptr)
			E_ERROR("Can not find shader name: " + string(pShader->Attribute("name")));
	}
	return true;
}

TerrainRenderComponent::~TerrainRenderComponent()
{
	for (int i = 0; i < m_MeshList.size(); i++)
	{
		delete m_MeshList[i];
	}
}

Mesh * TerrainRenderComponent::ReadFile(const char * filename)
{
	if (filename == nullptr) return nullptr;

	GLint width, height, iType, iBpp;

	ilLoadImage(filename);

	ILenum Error;
	Error = ilGetError();

	if (Error != IL_NO_ERROR)
	{
		Log::Message(Log::LOG_ERROR, "Can't load terrain " + string(filename));
		return nullptr;
	}
	width = ilGetInteger(IL_IMAGE_WIDTH);
	height = ilGetInteger(IL_IMAGE_HEIGHT);
	iType = ilGetInteger(IL_IMAGE_FORMAT);
	ILubyte *Data = ilGetData();
	iBpp = ilGetInteger(IL_IMAGE_BYTES_PER_PIXEL);

	float stepsize = 50;
	vec2 size = vec2(width*stepsize, height*stepsize);
	Mesh* p = new Mesh;

	int x = -size[0] / 2, y = 0, z = -size[1] / 2;

	// computer vertex xyz
	for (int i = 0; i < height; i++)
	{
		z += stepsize;
		for (int j = 0; j < width; j++)
		{
			x += stepsize;
			y = Data[i*width + j]*0.1;
			vec3 pos(x, y, z);
			vec2 uv((x + size[0] / 2) / size[0], (z + size[1] / 2) / size[1]);
			vec3 normal(0, 1, 0);
			DefaultVertex vertex{ pos,normal,uv };
			p->m_Vertexs.push_back(vertex);
		}
		x = -size[0] / 2;
	}
	// computer indices
	GLuint cnt = 0;
	for (int i = 0; i < height - 1; i++)
		for (int j = 0; j <width - 1; j++)
		{
			p->m_Indices.push_back(j + (i + 1)*width + 1);
			p->m_Indices.push_back(j + i*width + 1);
			p->m_Indices.push_back(j + i*width);

			p->m_Indices.push_back(j + (i + 1)*width);
			p->m_Indices.push_back(j + (i + 1)*width + 1);
			p->m_Indices.push_back(j + i*width);
		}

	// computer normal
	vec3 off = vec3(1.0, 1.0, 0.0);
	for (int i = 1; i < height - 1; i++)
		for (int j = 1; j < width - 1; j++)
		{
			//	vec2 v()
		}
	ilResetMemory();

	return p;
}
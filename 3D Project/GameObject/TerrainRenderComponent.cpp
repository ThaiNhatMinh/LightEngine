#include "pch.h"

#include <IL\il.h>
#include <IL\ilu.h>

const char* TerrainRenderComponent::Name = "TerrainRenderComponent";

TerrainRenderComponent::TerrainRenderComponent()
{
	
}

bool TerrainRenderComponent::VInit(const tinyxml2::XMLElement* pData)
{
	m_fScale = 10.0f;
	if (!pData) return false;
	
	Resources* pResources =  m_Context->GetSystem<Resources>();

	const tinyxml2::XMLElement* pModelPath = pData->FirstChildElement("Model");

	const char* pFileName = pModelPath->Attribute("File");
	if (pFileName)
	{
		HeightMap* hm =pResources->GetHeightMap(pFileName);
		Mesh* p = static_cast<Mesh*>(hm->m_Mesh[0].get());
		if (p)
		{
			const tinyxml2::XMLElement* pTexPath = pData->FirstChildElement("Texture");
			const char* pFileName1 = pTexPath->Attribute("File0");
			p->Tex = pResources->GetTexture(pFileName1);
			m_MeshList.push_back(p);
		}
	}

	const tinyxml2::XMLElement* pShader = pData->FirstChildElement("Shader");
	if (pShader)
	{
		m_pShader =pResources->GetShader(pShader->Attribute("name"));
		if (m_pShader == nullptr)
			E_ERROR("Can not find shader name: " + string(pShader->Attribute("name")));
	}
	return true;
}

void TerrainRenderComponent::Render(Scene *pScene)
{
	if (m_MeshList.empty()) return;
	m_pShader->SetupRender(pScene, m_pOwner);

	

	m_pShader->SetUniform("scale", m_fScale);

	m_pShader->SetUniform("gMaterial.Ka", m_Material.Ka);
	m_pShader->SetUniform("gMaterial.Kd", m_Material.Kd);
	m_pShader->SetUniform("gMaterial.Ks", m_Material.Ks);
	m_pShader->SetUniform("gMaterial.exp", m_Material.exp);

	for (size_t i = 0; i < m_MeshList.size(); i++)
	{
		m_MeshList[i]->Tex->Bind();

		// ------- Render mesh ----------
		m_pRenderer->SetVertexArrayBuffer(m_MeshList[i]->VAO);
		m_pRenderer->SetDrawMode(m_MeshList[i]->Topology);
		m_pRenderer->DrawElement(m_MeshList[i]->NumIndices, GL_UNSIGNED_INT, 0);
	}
}

TerrainRenderComponent::~TerrainRenderComponent()
{
	
}

/*
Mesh * TerrainRenderComponent::ReadFile(const string& filename)
{
	if (filename == nullptr) return nullptr;
	HeightMap* hm = m_Context->m_pResources->GetHeightMap(filename);
	/*GLint width, height, iType, iBpp;

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
	GLint width = hm.Width, height = hm.Height;
	float stepsize = hm.stepsize;
	GLubyte* Data = hm.Data;
	vec2 size = vec2((width-1)*stepsize, (height-1)*stepsize);
	Mesh* p = new Mesh;

	int x = -size[0] / 2, y = 0, z = -size[1] / 2;
	float t = (hm.minH + hm.maxH) / 2.0f;
	// computer vertex xyz
	for (int i = 0; i < height; i++)
	{
		
		for (int j = 0; j < width; j++)
		{
			int b = i*width + j;
			
			y = Data[b]-t;
			vec3 pos(x, y, z);
			vec2 uv((x + size[0] / 2) / size[0], (z + size[1] / 2) / size[1]);
			vec3 normal(0, 1, 0);
			DefaultVertex vertex{ pos,normal,uv };
			p->m_Vertexs.push_back(vertex);
			x += stepsize;
		}
		x = -size[0] / 2;
		z += stepsize;
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
	
	auto heightF = [Data,width,height](int x,int z) {
		if (x < 0) x = 0;
		if (z < 0) z = 0;
		if (x >= width) x = width;
		if (z >= height) z = height;
		int a = (int)(x*width + z);
		return Data[a];
	};
	
	int id = 0;
	for (int i = 0; i < height ; i++)
		for (int j = 0; j < width ; j++)
		{
			vec2 P(i, j);
			float hL = heightF(j-1,i);
			float hR = heightF(j+1,i);
			float hD = heightF(j,i-1);
			float hU = heightF(j,i+1);
			vec3 N;
			N.x = hL - hR;
			N.y = hD - hU;
			N.z = 2.0;
			N = normalize(N);
			p->m_Vertexs[id].normal = N;
			id++;
		}

	return p;
}*/ 

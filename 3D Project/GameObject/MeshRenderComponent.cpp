#include "pch.h"

const char* MeshRenderComponent::Name = "MeshRenderComponent";

void MeshRenderComponent::VPostInit(void)
{
}

bool MeshRenderComponent::VInit(const tinyxml2::XMLElement* pData)
{
	if(!pData) return false;

	const tinyxml2::XMLElement* pModelPath = pData->FirstChildElement("Model");
	
	const char* pFileName = pModelPath->Attribute("File");
	if (pFileName)
	{
		if (pFileName[0] != '\0')
		{
			ModelCache* pModel = m_Context->m_pResources->GetModel(pFileName);
			if (!pModel) return false;
			
			SetData(pModel);
		}
	}
	else
	{
		GLuint pShape = pModelPath->DoubleAttribute("Shape");
		float size[3];
		size[0] = pModelPath->DoubleAttribute("sx");
		size[1] = pModelPath->DoubleAttribute("sy");
		size[2] = pModelPath->DoubleAttribute("sz");

		const tinyxml2::XMLElement* pTexPath = pData->FirstChildElement("Texture");
		const char* pFileName1 = pTexPath->Attribute("File");

		IMesh* p = m_Context->m_pResources->CreateShape((ShapeType)pShape, size);

		p->Tex = m_Context->m_pResources->GetTexture(pFileName1);
		m_MeshList.push_back(p);
	}
	

	const tinyxml2::XMLElement* pShader = pData->FirstChildElement("Shader");
	if (pShader)
	{
		m_pShader = m_Context->m_pResources->GetShader(pShader->Attribute("name"));
		if (m_pShader == nullptr)
			E_ERROR("Can not find shader name: " + string(pShader->Attribute("name")));
	}

	

	return true;

}

tinyxml2::XMLElement * MeshRenderComponent::VGenerateXml(tinyxml2::XMLDocument * p)
{
	tinyxml2::XMLElement* pBaseElement = p->NewElement(VGetName());
	tinyxml2::XMLElement* pModelPath = p->NewElement("Model");
	pModelPath->SetAttribute("File", "");
	return nullptr;
}

void MeshRenderComponent::Render(Scene* pScene)
{
	if (m_MeshList.empty()) return;
	m_pShader->SetupRender(pScene, m_pOwner);

	RenderAPICore* pRender = m_Context->m_pRenderer.get();

	
	m_pShader->SetUniform("gMaterial.Ka", m_Material.Ka);
	m_pShader->SetUniform("gMaterial.Kd", m_Material.Kd);
	m_pShader->SetUniform("gMaterial.Ks", m_Material.Ks);
	m_pShader->SetUniform("gMaterial.exp", m_Material.exp);

	for (size_t i = 0; i < m_MeshList.size(); i++)
	{
		m_MeshList[i]->Tex->Bind();

		// ------- Render mesh ----------
		pRender->SetVertexArrayBuffer(m_MeshList[i]->VAO);
		pRender->SetDrawMode(m_MeshList[i]->Topology);
		pRender->DrawElement(m_MeshList[i]->NumIndices, GL_UNSIGNED_INT, 0);
	}


}

vector<IMesh*>& MeshRenderComponent::GetMeshList()
{
	return m_MeshList;
}

vector<LTBSocket>& MeshRenderComponent::GetSockets()
{
	assert(m_Model != nullptr);

	return m_Model->Sockets;
}

vector<std::unique_ptr<SkeNode>>& MeshRenderComponent::GetNodeList()
{
	return m_Model->pSkeNodes;
}

void MeshRenderComponent::SetData(ModelCache * pModel)
{
	m_Model = pModel;
	for (size_t i = 0; i < m_Model->pMeshs.size(); i++)
	{
		m_MeshList.push_back(m_Model->pMeshs[i].get());
	}

}

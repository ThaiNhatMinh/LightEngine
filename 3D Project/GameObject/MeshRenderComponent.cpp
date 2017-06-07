#include "..\pch.h"

const char* MeshRenderComponent::Name = "MeshRenderComponent";

void MeshRenderComponent::VPostInit(void)
{
	Shader* p = m_pOwner->VGetShader();
	for (vector<IMesh*>::iterator it = m_MeshList.begin(); it != m_MeshList.end(); it++)
	{
		(*it)->Finalize(p);
	}
}

bool MeshRenderComponent::VInit(tinyxml2::XMLElement * pData)
{
	if(!pData) return false;

	tinyxml2::XMLElement* pModelPath = pData->FirstChildElement("Model");
	
	const char* pFileName = pModelPath->Attribute("File");
	ModelCache* pModel = gResources()->LoadModelXML(pFileName);
	if (!pModel)
	{
		return false;
	}

	for (size_t i = 0; i < pModel->pMeshs.size(); i++)
	{
		m_MeshList.push_back(pModel->pMeshs[i]);
	}
	return true;

}

tinyxml2::XMLElement * MeshRenderComponent::VGenerateXml(tinyxml2::XMLDocument * p)
{
	// Nothing here. 
	// Update late.
	return nullptr;
}

void MeshRenderComponent::Render()
{
	for (size_t i = 0; i < m_MeshList.size(); i++)
	{
		if(m_MeshList[i]->Tex) m_MeshList[i]->Tex->Bind(0);
		glBindVertexArray(m_MeshList[i]->VAO);
		glDrawElements(m_MeshList[i]->Topology, m_MeshList[i]->NumIndices, GL_UNSIGNED_INT, 0);
	}
}

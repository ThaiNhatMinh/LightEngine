#include "..\pch.h"

const char* MeshRenderComponent::Name = "MeshRenderComponent";

bool MeshRenderComponent::VInit(tinyxml2::XMLElement * pData)
{
	if(!pData) return false;

	tinyxml2::XMLElement* pModelPath = pData->FirstChildElement("Model");
	
	const char* pFileName = pModelPath->Attribute("File");
	ModelCache* pModel = gResources()->LoadModel(pFileName);
	if (!pModel)
	{
		return false;
	}
	

	tinyxml2::XMLElement* pMaterialData = pData->FirstChildElement("Material");
	Material mat;
	tinyxml2::XMLElement* pKa = pMaterialData->FirstChildElement("Ka");
	mat.Ka.x = pKa->FloatAttribute("r", 1.0f);
	mat.Ka.y = pKa->FloatAttribute("g", 1.0f);
	mat.Ka.z = pKa->FloatAttribute("b", 1.0f);
	tinyxml2::XMLElement* pKd = pMaterialData->FirstChildElement("Kd");
	mat.Kd.x = pKd->FloatAttribute("r", 1.0f);
	mat.Kd.y = pKd->FloatAttribute("g", 1.0f);
	mat.Kd.z = pKd->FloatAttribute("b", 1.0f);
	tinyxml2::XMLElement* pKs = pMaterialData->FirstChildElement("Ks");
	mat.Ks.x = pKs->FloatAttribute("r", 1.0f);
	mat.Ks.y = pKs->FloatAttribute("g", 1.0f);
	mat.Ks.z = pKs->FloatAttribute("b", 1.0f);
	mat.exp = pKs->FloatAttribute("exp", 1.0f);

	// Get mesh list and update Material 
	for (size_t i = 0; i < pModel->pMeshs.size(); i++)
	{
		pModel->pMeshs[i]->mat = mat;
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
		m_MeshList[i]->Tex->Bind(0);
		glBindVertexArray(m_MeshList[i]->VAO);
		glDrawElements(m_MeshList[i]->Topology, m_MeshList[i]->NumIndices, GL_UNSIGNED_INT, 0);
	}
}

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
	if (pFileName)
	{
		ModelCache* pModel = gResources()->LoadModelXML(pFileName);
		if (!pModel)
		{
			return false;
		}
		for (size_t i = 0; i < pModel->pMeshs.size(); i++)
		{
			m_MeshList.push_back(pModel->pMeshs[i]);
		}
		m_Material = pModel->mat;
	}
	else if(pFileName = pModelPath->Attribute("Shape"))
	{
		m_MeshList.push_back(gResources()->CreateShape(CUBE));
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
	Shader* p = m_pOwner->VGetShader();

	for (size_t i = 0; i < m_MeshList.size(); i++)
	{
		if(m_MeshList[i]->Tex) m_MeshList[i]->Tex->Bind(0);
		else p->SetUniform("ObjColor", m_MeshList[i]->Color);
		glBindVertexArray(m_MeshList[i]->VAO);
		glDrawElements(m_MeshList[i]->Topology, m_MeshList[i]->NumIndices, GL_UNSIGNED_INT, 0);
	}
}

Material MeshRenderComponent::GetMaterial()
{
	return m_Material;
}

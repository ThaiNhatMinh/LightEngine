#include "pch.h"

const char* MeshRenderComponent::Name = "MeshRenderComponent";

void MeshRenderComponent::VPostInit(void)
{
	for (vector<IMesh*>::iterator it = m_MeshList.begin(); it != m_MeshList.end(); it++)
	{
		(*it)->Finalize(m_pShader);
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

tinyxml2::XMLElement * MeshRenderComponent::VGenerateXml(tinyxml2::XMLDocument * p)
{
	// Nothing here. 
	// Update late.
	return nullptr;
}

void MeshRenderComponent::Render(Scene* pScene)
{

	m_pShader->SetupRender(pScene, m_pOwner);

	RenderAPICore* pRender = pScene->GetRenderer();

	for (size_t i = 0; i < m_MeshList.size(); i++)
	{
		if (m_MeshList[i]->Tex) pRender->SetTexture(m_MeshList[i]->Tex);
		else m_pShader->SetUniform("ObjColor", m_MeshList[i]->Color);

		pRender->SetVertexArrayBuffer(m_MeshList[i]->VAO);
		pRender->SetDrawMode(m_MeshList[i]->Topology);
		pRender->DrawElement(m_MeshList[i]->NumIndices, GL_UNSIGNED_INT, 0);
	}

	if (1)
	{
		mat4 globalTransform = m_pOwner->VGetGlobalTransform();
		AnimationComponent* ac = m_pOwner->GetComponent<AnimationComponent>("AnimationComponent");
		if (ac) ac->DrawSkeleton(pScene->GetDebug(), globalTransform);
	}

}

Material MeshRenderComponent::GetMaterial()
{
	return m_Material;
}

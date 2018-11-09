#include <pch.h>
#include "MeshRenderComponent.h"
#include "..\Interface\IResourceManager.h"
#include "..\Interface\IRenderSystem.h"
namespace Light
{
	MeshRenderComponent::MeshRenderComponent()
	{
		this->m_pModel = nullptr;
	}
	bool MeshRenderComponent::VSerialize(IContext*pContext,const tinyxml2::XMLElement * pData)
	{
		if (!pData) return false;

		const tinyxml2::XMLElement* pModelPath = pData->FirstChildElement("Model");

		auto pRenderS = pContext->GetSystem<render::IRenderSystem>();
		auto pResourceM = pContext->GetSystem<resources::IResourceManager>();
		const char* pFileName = pModelPath->Attribute("File");
		
		if (pFileName)
		{
			if (pFileName[0] != '\0')
			{
				std::string file(pFileName);
				render::Model* pModel = NULL;
				if(file.find(".xml")==std::string::npos)
					pModel = pRenderS->VCreateModel(pResourceM->VGetModel(pFileName));
				else pModel = pRenderS->VCreateModel(file);

				if (!pModel)
					return false;

				this->m_pModel = pModel;
			}
		}
		
		return true;

	}

	tinyxml2::XMLElement * MeshRenderComponent::VDeserialize(tinyxml2::XMLDocument * p)
	{
		tinyxml2::XMLElement* pBaseElement = p->NewElement("MeshRenderComponent");
		tinyxml2::XMLElement* pModelPath = p->NewElement("Model");
		pModelPath->SetAttribute("File", "");

		pBaseElement->InsertFirstChild(pModelPath);

		return pBaseElement;
	}

	//void MeshRenderComponent::Render(Scene* pScene)
	//{
	//	if (m_MeshList.empty()) return;
	//	m_pShader->SetupRender(pScene, m_pOwner);



	//	for (size_t i = 0; i < m_MeshList.size(); i++)
	//	{
	//		// Set material
	//		m_pShader->SetUniform("gMaterial.Ka", m_MeshList[i]->mat.Ka);
	//		m_pShader->SetUniform("gMaterial.Kd", m_MeshList[i]->mat.Kd);
	//		m_pShader->SetUniform("gMaterial.Ks", m_MeshList[i]->mat.Ks);
	//		m_pShader->SetUniform("gMaterial.exp", m_MeshList[i]->mat.exp);

	//		m_MeshList[i]->Tex->Bind();

	//		// ------- Render mesh ----------
	//		m_MeshList[i]->VAO.Bind();
	//		m_pRenderer->SetDrawMode(m_MeshList[i]->Topology);
	//		m_pRenderer->DrawElement(m_MeshList[i]->NumIndices, GL_UNSIGNED_INT, 0);
	//	}

	//	//glEnable(GL_CULL_FACE);

	//}
}

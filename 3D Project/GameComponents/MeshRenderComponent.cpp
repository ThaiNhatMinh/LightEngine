#include <pch.h>
#include "MeshRenderComponent.h"
namespace Light
{
	MeshRenderComponent::MeshRenderComponent(void)
	{
	}

	bool MeshRenderComponent::VSerialize(const tinyxml2::XMLElement* pData)
	{
		if (!pData) return false;

		/*const tinyxml2::XMLElement* pModelPath = pData->FirstChildElement("Model");

		const char* pFileName = pModelPath->Attribute("File");
		if (pFileName)
		{
			if (pFileName[0] != '\0')
			{
				IModelResource* pModel = m_Context->GetSystem<Resources>()->GetModel(pFileName);
				if (!pModel)
					return false;

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

			IMesh* p = m_Context->GetSystem<Resources>()->CreateShape((ShapeType)pShape, size);

			p->Tex = m_Context->GetSystem<Resources>()->GetTexture(pFileName1);
			m_MeshList.push_back(p);
		}


		const tinyxml2::XMLElement* pShader = pData->FirstChildElement("Shader");
		if (pShader)
		{
			m_pShader = m_Context->GetSystem<Resources>()->GetShader(pShader->Attribute("name"));
			if (m_pShader == nullptr)
				E_ERROR("Can not find shader name: " + string(pShader->Attribute("name")));
		}*/



		return true;

	}

	tinyxml2::XMLElement * MeshRenderComponent::VDeserialize(tinyxml2::XMLDocument * p)
	{
		tinyxml2::XMLElement* pBaseElement = p->NewElement("MeshRenderComponent");
		tinyxml2::XMLElement* pModelPath = p->NewElement("Model");
		pModelPath->SetAttribute("File", "");
		return nullptr;
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

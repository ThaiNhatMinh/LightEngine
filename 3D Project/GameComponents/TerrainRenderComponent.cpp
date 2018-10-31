#include <pch.h>
#include "TerrainRenderComponent.h"
#include "..\Math\Math.h"
#include "..\Interface\IRenderSystem.h"

namespace Light
{
	bool TerrainRenderComponent::VSerialize(IContext* pContext, const tinyxml2::XMLElement * pData)
	{
		m_fScale = 20.0f;
		if (!pData) return false;
		const tinyxml2::XMLElement* pModelPath = pData->FirstChildElement("Model");

		const char* pFileName = pModelPath->Attribute("File");
		if (pFileName)
		{
			auto pResources = pContext->GetSystem<resources::IResourceManager>();
			auto pRenderDevice = pContext->GetSystem<render::IRenderSystem>()->GetRenderDevice();
			resources::HeightMap* hm = pResources->VGetHeightMap(pFileName);

			const tinyxml2::XMLElement* pTexPath = pData->FirstChildElement("Texture");
			const char* pFileName1 = pTexPath->Attribute("File0");
			

			GenerateMeshData(pRenderDevice, pContext->GetSystem<IFactory>(),hm, pResources->VGetTexture(pFileName1));
			
			return true;
		}
		return false;
	}

	tinyxml2::XMLElement * TerrainRenderComponent::VDeserialize(tinyxml2::XMLDocument * p)
	{
		return nullptr;
	}

	//void TerrainRenderComponent::Render(Scene *pScene)
	//{
	//	if (m_MeshList.empty()) return;
	//	m_pShader->SetupRender(pScene, m_pOwner);



	//	m_pShader->SetUniform("scale", m_fScale);

	//	m_pShader->SetUniform("gMaterial.Ka", m_Material.Ka);
	//	m_pShader->SetUniform("gMaterial.Kd", m_Material.Kd);
	//	m_pShader->SetUniform("gMaterial.Ks", m_Material.Ks);
	//	m_pShader->SetUniform("gMaterial.exp", m_Material.exp);

	//	ICamera* pCam = pScene->GetCurrentCamera();
	//	Frustum* pFrustum = pCam->GetFrustum();
	//	int numdraw = 0;
	//	//glPolygonMode(GL_FRONT, GL_LINE);
	//	for (size_t i = 0; i < m_MeshList.size(); i++)
	//	{
	//		SubGrid* pGrid = static_cast<SubGrid*>(m_MeshList[i]);

	//		if (!pFrustum->Inside(pGrid->box.Min, pGrid->box.Max)) continue;

	//		m_MeshList[i]->Tex->Bind();

	//		// ------- Render mesh ----------
	//		//m_pRenderer->SetVertexArrayBuffer(m_MeshList[i]->VAO);
	//		m_MeshList[i]->VAO.Bind();
	//		m_pRenderer->SetDrawMode(m_MeshList[i]->Topology);
	//		m_pRenderer->DrawElement(m_MeshList[i]->NumIndices, GL_UNSIGNED_INT, 0);
	//		numdraw++;
	//	}

	//	ImGui::Text("Num SubGrid draw: %d in total %d", numdraw, m_MeshList.size());


	//}

	TerrainRenderComponent::~TerrainRenderComponent()
	{

	}

	void TerrainRenderComponent::GenerateMeshData(render::RenderDevice* pRenderDevice, IFactory* pFactory, resources::HeightMap * hm, render::Texture* pText)
	{
		auto pModel = DEBUG_NEW TerrainModel();
		this->m_pModel = pModel;
		math::AABB box;
		std::size_t numMesh = hm->numSub;			// Num SubMesh device by row and collum
		std::size_t numvert = hm->Width / numMesh;	// Num vertices per SubMesh in Row/Collum

		int xpos = 0, zpos = 0;
		int pos[2] = { xpos,zpos };

		std::vector<std::vector<DefaultVertex>> vertexList;
		for (int i = 0; i < numMesh; i++)
		{
			for (int j = 0; j < numMesh; j++)
			{
				std::vector<DefaultVertex> vertex;
				vertex = Light::math::CopySubMatrix(hm->m_Vertexs, pos, numvert);
				vertexList.push_back(vertex);
				pos[0] += numvert - 1;
			}
			pos[0] = 0;
			pos[1] += numvert - 1;
		}

		std::vector<unsigned int> Index;
		std::size_t cnt = 0;
		for (std::size_t i = 0; i < numvert - 1; i++)
			for (std::size_t j = 0; j < numvert - 1; j++)
			{
				Index.push_back(j + (i + 1)*numvert + 1);
				Index.push_back(j + i * numvert + 1);
				Index.push_back(j + i * numvert);

				Index.push_back(j + (i + 1)*numvert);
				Index.push_back(j + (i + 1)*numvert + 1);
				Index.push_back(j + i * numvert);
			}
		for (int i = 0; i < vertexList.size(); i++)
		{
			auto pTemp = DEBUG_NEW SubGrid(pRenderDevice, vertexList[i], Index);
			pModel->Meshs.push_back(std::unique_ptr<Mesh>(pTemp));
			box.Test(pTemp->box.Min);
			box.Test(pTemp->box.Max);
		}

		//pModel->Textures = pText;
		pModel->Material = pFactory->VGetMaterial("Default");
		pModel->box = box;
		pModel->Textures = pText;
	}



	TerrainRenderComponent::SubGrid::SubGrid(render::RenderDevice* pRenderDevice,const std::vector<DefaultVertex>& vertex, const std::vector<unsigned int> indices) :box(),DefaultMesh(pRenderDevice,vertex,indices,"Subgrid")
	{
		for (size_t i = 0; i < vertex.size(); i++) box.Test(vertex[i].pos);

	}

	void TerrainRenderComponent::TerrainModel::Draw(render::RenderData& rd, render::Material::MatrixParam & matrixParam)
	{
		//int numdraw = 0;
		auto pFrustum = rd.pCamera->GetFrustum();
		Material->Apply(rd.pRenderer, matrixParam);
		rd.pRenderer->SetTexture(render::UNIT_DIFFUSE, Textures);
		for (std::size_t i = 0; i < Meshs.size(); i++)
		{
			SubGrid* pGrid = static_cast<SubGrid*>(Meshs[i].get());

			if (!pFrustum->Inside(pGrid->box.Min, pGrid->box.Max)) continue;
			Meshs[i]->Draw(rd.pRenderer);
			//numdraw++;
		}
		//cout << numdraw << endl;
	}

	render::MeshList & TerrainRenderComponent::TerrainModel::GetMeshs()
	{
		return Meshs;
	}
	math::AABB TerrainRenderComponent::TerrainModel::GetBox()
	{
		return box;
	}
}
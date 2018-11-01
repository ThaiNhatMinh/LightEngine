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

		auto pParam = pData->FirstChildElement("Param");
		stepsize = pParam->Int64Attribute("StepSize", 50);
		hscale = pParam->Int64Attribute("HScale", 10);
		numSub = pParam->Int64Attribute("NumSub", 2);

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

	TerrainRenderComponent::~TerrainRenderComponent()
	{

	}

	void TerrainRenderComponent::GenerateMeshData(render::RenderDevice* pRenderDevice, IFactory* pFactory, resources::HeightMap * hm, render::Texture* pText)
	{
		auto pModel = DEBUG_NEW TerrainModel();
		this->m_pModel = pModel;
		math::AABB box;
		std::size_t numMesh = numSub;			// Num SubMesh device by row and collum
		std::size_t numvert = hm->Width / numMesh;	// Num vertices per SubMesh in Row/Collum

		int xpos = 0, zpos = 0;
		int pos[2] = { xpos,zpos };

		auto vertexs = math::GenerateVertexData(hm, stepsize, hm->Width, hm->Height, hscale, numSub);
		std::vector<std::vector<DefaultVertex>> vertexList;
		for (int i = 0; i < numMesh; i++)
		{
			for (int j = 0; j < numMesh; j++)
			{
				std::vector<DefaultVertex> vertex;
				vertex = Light::math::CopySubMatrix(vertexs, pos, numvert);
				vertexList.push_back(vertex);
				pos[0] += numvert - 1;
			}
			pos[0] = 0;
			pos[1] += numvert - 1;
		}

		std::vector<unsigned int> Index = math::GenerateIndicesData(hm,numSub);
		
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
#include <pch.h>
#include <time.h>
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
		if (!pFileName) return 0;

		auto pResources = pContext->GetSystem<resources::IResourceManager>();
		auto pRenderDevice = pContext->GetSystem<render::IRenderSystem>()->GetRenderDevice();
		auto pFactory = pContext->GetSystem<IFactory>();

		resources::HeightMap* hm = pResources->VGetHeightMap(pFileName);

		const tinyxml2::XMLElement* pTexPath = pData->FirstChildElement("Texture");
		

		auto pModel = GenerateMeshData(pRenderDevice, hm);
		//pModel->Textures = pText;
		pModel->Material = pFactory->VGetMaterial("Default")->Clone();
		auto ppipeline = pRenderDevice->CreatePipeline(pResources->VGetVertexShader("Terrain"), pResources->VGetPixelShader("Terrain"));
		pModel->Material->SetPipeline(ppipeline);
		pModel->Material->ClearTextureData();
		LoadTexture(pTexPath, pResources, pModel);
		LoadObject(pData->FirstChildElement("Assets"), pResources, pModel, hm);
		this->m_pModel = pModel;
		return true;


		
	}

	tinyxml2::XMLElement * TerrainRenderComponent::VDeserialize(tinyxml2::XMLDocument * p)
	{
		return nullptr;
	}

	TerrainRenderComponent::~TerrainRenderComponent()
	{
		delete m_pModel;
	}

	TerrainRenderComponent::TerrainModel* TerrainRenderComponent::GenerateMeshData(render::RenderDevice* pRenderDevice, resources::HeightMap * hm)
	{
		auto pModel = DEBUG_NEW TerrainModel();
		
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

		pModel->box = box;

		return pModel;
		
	}

	void TerrainRenderComponent::LoadTexture(const tinyxml2::XMLElement * pData, resources::IResourceManager * pResources, TerrainModel* pModel)
	{
		int i = 0;
		for (auto pNode = pData->FirstChildElement("File"); pNode; pNode = pNode->NextSiblingElement("File"))
		{
			auto pFile = pNode->GetText();
			pModel->Textures.push_back(std::pair<render::TextureUnit, render::Texture*>((render::TextureUnit)pNode->Int64Attribute("unit",i),pResources->VGetTexture(pFile)));
			pModel->Material->AddTexUnit(pNode->Attribute("uniform"), (render::TextureUnit)pNode->Int64Attribute("unit", i));
			i++;
		}
		
		auto pFile = pData->FirstChildElement("Blend")->GetText();
		pModel->Blend = pResources->VGetTexture(pFile);
		pModel->Material->AddTexUnit("mat.blend",render::UNIT_BLEND);
	}

	void TerrainRenderComponent::LoadObject(const tinyxml2::XMLElement * pData, resources::IResourceManager * pResources, TerrainModel * pModel, resources::HeightMap * hm)
	{
		
		for (auto pNode = pData->FirstChildElement(); pNode; pNode = pNode->NextSiblingElement())
		{
			int num = pNode->Int64Attribute("num");
			auto pFile = pNode->GetText();
			TerrainObject obj;
			
			obj.model = pResources->VGetModel(pFile);
			if (!obj.model) continue;
			for (int i = 0; i < num; i++)
			{
				glm::mat4 scale = glm::scale(glm::mat4(),glm::vec3(2));
				glm::vec3 pos = RandomPos(hm);
				glm::mat4 translate = glm::translate(glm::mat4(),pos);
				obj.transform = translate* scale;
				pModel->m_Objects.push_back(obj);
			}

		}
	}

	glm::vec3 TerrainRenderComponent::RandomPos(resources::HeightMap * hm)
	{
		vec2 size = vec2((hm->Width - 1)*stepsize, (hm->Height - 1)*stepsize);

		vec2 v = vec2(rand() % hm->Width, rand() % hm->Height);

		float t = (hm->minH + hm->maxH) / 2.0f;
		int y = (hm->Data[v.y*hm->Width + v.x] - t)*hscale;

		return glm::vec3(v.x*stepsize-size[0] / 2,y,v.y*stepsize - size[1] / 2);
	}

	



	TerrainRenderComponent::SubGrid::SubGrid(render::RenderDevice* pRenderDevice,const std::vector<DefaultVertex>& vertex, const std::vector<unsigned int> indices) :box(),DefaultMesh(pRenderDevice,vertex,indices,"Subgrid")
	{
		for (size_t i = 0; i < vertex.size(); i++) box.Test(vertex[i].pos);

	}

	void TerrainRenderComponent::TerrainModel::Draw(render::RenderData& rd, render::Material::MatrixParam & matrixParam)
	{
		//int numdraw = 0;
		auto pFrustum = rd.pCamera->GetFrustum();
		render::MaterialData data;
		data.Ks = vec3(0);
		data.Kd = vec3(0.5f);
		//data.Ka = vec3(0.3f);
		data.exp = vec3(32);
		Material->Apply(rd.pRenderer, matrixParam, data);
		for(auto el:Textures)
			rd.pRenderer->SetTexture(el.first, el.second);

		rd.pRenderer->SetTexture(render::UNIT_BLEND, Blend);

		for (std::size_t i = 0; i < Meshs.size(); i++)
		{
			SubGrid* pGrid = static_cast<SubGrid*>(Meshs[i].get());

			if (!pFrustum->Inside(pGrid->box.Min, pGrid->box.Max)) continue;
			Meshs[i]->Draw(rd.pRenderer);
			//numdraw++;
		}

		for (int i=0; i<m_Objects.size(); i++)
		{
			math::AABB box = m_Objects[i].model->GetBox();
			box = math::TrasformAABB(box, m_Objects[i].transform);
			matrixParam[render::uMODEL] = glm::value_ptr(m_Objects[i].transform);
			matrixParam[render::uMVP] = glm::value_ptr(rd.pv*m_Objects[i].transform);
			if (!pFrustum->Inside(box.Min, box.Max)) continue;
			m_Objects[i].model->Draw(rd, matrixParam);
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
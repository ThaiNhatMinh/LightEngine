#pragma once
#include "..\Interface\IComponent.h"
#include "..\Interface\IResourceManager.h"
#include "..\Interface\IFactory.h"
#include "..\Interface\IRenderSystem.h"
#include "..\Graphics3D\DefaultMesh.h"
#include "..\Math\AABB.h"
namespace Light
{

	class TerrainRenderComponent : public IMeshRenderComponent
	{
	private:
		struct SubGrid : public DefaultMesh
		{
			math::AABB box;
			SubGrid(render::RenderDevice* pRenderDevice,const std::vector<DefaultVertex>& vertex, const std::vector<unsigned int> indices);
		};

		struct TerrainObject
		{
			glm::mat4 transform; // final transform 
			render::Model* model;
		};
		struct TerrainModel : public render::Model
		{
			std::vector<TerrainObject> m_Objects;
			std::vector<std::unique_ptr<Mesh>>  Meshs;
			std::vector<std::pair<render::TextureUnit,render::Texture*>>		Textures;
			render::Texture*					Blend;
			std::shared_ptr<render::Material>	Material;
			math::AABB box;
			virtual void Draw(render::RenderData& rd, render::Material::MatrixParam& matrixParam)override;

			virtual render::MeshList& GetMeshs()override;
			virtual math::AABB GetBox()override;
		};
		float stepsize;
		float hscale;
		
		uint32 numSub;

		float m_fScale;
		//Material m_Material;
	public:
		
		virtual bool VSerialize(IContext*pContext, const tinyxml2::XMLElement * pData);
		virtual tinyxml2::XMLElement* VDeserialize(tinyxml2::XMLDocument*p);
		~TerrainRenderComponent();
	private:
		TerrainModel* GenerateMeshData(render::RenderDevice* pRenderDevice,resources::HeightMapData * hm);
		void LoadTexture(const tinyxml2::XMLElement * pData,render::IRenderSystem* pRS, resources::IResourceManager* pResources, TerrainModel* pModel);
		void LoadObject(const tinyxml2::XMLElement * pData, render::IRenderSystem* pRS, resources::IResourceManager* pResources, TerrainModel* pModel, resources::HeightMapData * hm);
		glm::vec3 RandomPos(resources::HeightMapData * hm);
	};

}
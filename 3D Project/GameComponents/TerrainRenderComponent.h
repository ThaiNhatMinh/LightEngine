#pragma once
#include "..\Interface\IComponent.h"
#include "..\Math\AABB.h"
#include "..\Interface\IResourceManager.h"
#include "..\Interface\IFactory.h"
#include "..\Graphics3D\DefaultMesh.h"
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

		struct TerrainModel : public render::Model
		{
			std::vector<std::unique_ptr<Mesh>>  Meshs;
			render::Texture* Textures;
			std::shared_ptr<render::Material>	Material;
			math::AABB box;
			virtual void Draw(render::RenderData& rd, render::Material::MatrixParam& matrixParam)override;

			virtual render::MeshList& GetMeshs()override;
			virtual math::AABB GetBox()override;
		};
		float m_fScale;
		//Material m_Material;
	public:
		
		virtual bool VSerialize(IContext*pContext, const tinyxml2::XMLElement * pData);
		virtual tinyxml2::XMLElement* VDeserialize(tinyxml2::XMLDocument*p);
		~TerrainRenderComponent();
	private:
		void GenerateMeshData(render::RenderDevice* pRenderDevice, IFactory*,resources::HeightMap * hm, render::Texture* pText);
	};

}
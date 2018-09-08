#pragma once
#include <string>
#include <vector>
#include <glm/gtc/quaternion.hpp>
#include <glm/mat4x4.hpp>
#include "../typedef.h"
#include "../Graphics3D/SkeMesh.h"
#include "../Graphics3D/Renderer.h"
#include "../Graphics3D/Material.h"
namespace Light
{
	
	class LTModel :public render::Model
	{
	public:

		std::vector<std::unique_ptr<Mesh>>  Meshs;
		std::vector<render::Texture*> Textures;
		std::vector<std::shared_ptr<render::Material>>	Materials;

		/*std::vector<LTRawData>					Meshs;
		std::vector<SkeNode>						SkeNodes;
		std::vector<WeightBlend>					wb;
		std::vector<std::string>						ChildName;
		std::vector<Animation>					Anims;
		std::vector<LTBSocket>					Sockets;*/
		virtual void Draw(render::RenderDevice* pRenderer, const float * model, const float * mvp)override;
		virtual MeshList& GetMeshs()override;
	};
}

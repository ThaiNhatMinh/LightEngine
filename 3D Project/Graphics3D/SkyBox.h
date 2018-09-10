#pragma once

#include "Renderer.h"
#include "..\Utilities\Utility.h"

#include <memory>
#include <vector>
#include <string>
namespace Light
{
	namespace render
	{
		// base skybox object
		class SkyBox: public util::Serialization
		{
		public:
			SkyBox();
			virtual ~SkyBox() = default;

			virtual bool VSerialize(IContext*pContext, const tinyxml2::XMLElement * pData);
			virtual tinyxml2::XMLElement* VDeserialize(tinyxml2::XMLDocument*p);

			virtual void VUpdate(float dt) {};
			virtual void VRender();

			Texture* GetTexture();
		private:
			std::vector<std::string> GetFileList(std::string folder, const tinyxml2::XMLElement *pData);
		private:
			RenderDevice* m_pRenderer;
			std::unique_ptr<DepthStencilState> m_DepthState;
			std::unique_ptr<RasterState> m_CullState;
			std::unique_ptr<VertexArray> m_VAO;
			std::unique_ptr<VertexBuffer> m_VBO;
			std::unique_ptr<IndexBuffer> m_IBO;
			std::unique_ptr<Pipeline> m_Shader;

			PipelineParam* m_uMVP;
			Texture* m_CubeTex;
		};
	}
}
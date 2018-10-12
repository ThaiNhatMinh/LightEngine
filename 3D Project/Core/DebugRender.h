#pragma once
#include <glm/vec3.hpp>
#include <vector>
#include <memory>
#include "..\Interface\IRenderSystem.h"

namespace Light
{
	class DebugRender: public IDebugRender
	{
	private:

		struct DebugData
		{
			glm::vec3 pos;
			glm::vec3 color;
		};
		struct LightData
		{
			glm::mat4 model;
			glm::vec3 color;
		};
	public:
		DebugRender(IContext* pContext);
		virtual void DrawLine(const vec3& from, const vec3& to, const vec3& color = glm::vec3(1, 1, 1), const mat4& m = mat4())override;
		virtual void DrawLineBox(vec3 min, vec3 max, vec3 color = vec3(0.5f), const mat4& m = mat4())override;
		virtual void DrawCoord(const mat4& m)override;
		virtual void AddLightBox(glm::vec3 pos, glm::vec3 color)override;
		virtual void Render()override;
		virtual const char* VGetName()override;
	private:
		void SetupBoxRender(resources::IResourceManager*pResources);
		void SetupLineRender(resources::IResourceManager*pResources);
		void RenderLight(const glm::mat4& pv);
	private:
		
		render::RenderDevice* m_pRenderer;
		render::IRenderSystem* m_pRS;
		std::unique_ptr<render::DepthState> m_DepthConfig;
		std::unique_ptr<render::RasterState> m_CullConfig;

		// for light render
		std::vector<LightData> m_Lights;
		render::PipelineParam* m_uMVPLight;
		render::PipelineParam* m_uModelLight;
		render::PipelineParam* m_uColor;
		std::unique_ptr<render::Pipeline> m_ShaderLight;
		std::unique_ptr<render::VertexArray> m_VAOLightBox;
		std::unique_ptr<render::VertexBuffer> m_VBOLightBox;
		std::unique_ptr<render::IndexBuffer> m_IBOLightBox;

		// for line render
		std::vector<DebugData> m_Lists;
		render::PipelineParam* m_uMVP;
		std::unique_ptr<render::Pipeline> m_Shader;
		std::unique_ptr<render::VertexArray> m_VAO;
		std::unique_ptr<render::VertexBuffer> m_VBO;

	};
}
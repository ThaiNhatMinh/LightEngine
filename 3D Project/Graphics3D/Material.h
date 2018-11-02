#pragma once
#include <string>
#include <memory>
#include <map>

#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include "..\Utilities\Utility.h"
#include "..\typedef.h"
#include "..\Interface\Renderer.h"

namespace Light
{
	namespace render
	{
		class Pipeline;
		class RenderDevice;
		class PipelineParam;

		struct LightParam
		{
		private:
			PipelineParam* Ia;
			PipelineParam* Id;
			PipelineParam* Is;
			std::map<const char*, PipelineParam*> m_Extra;
		public:
			PipelineParam* operator[](const char*);
			const PipelineParam* operator[](const char*)const;
			void AddParam(const char* name, PipelineParam* param);
		};

		struct MaterialData
		{
			glm::vec3 Ka = glm::vec3(1.0f);
			glm::vec3 Kd = glm::vec3(1.0f);
			glm::vec3 Ks = glm::vec3(1.0f);
			glm::vec3 exp = glm::vec3(64.0f);
			std::string Name;

		};
		class Material
		{
			friend class ResourceManager;
		public:
			typedef std::map<const char*, const float*> MatrixParam;
		protected:
			std::vector<std::pair<render::PipelineParam*, render::TextureUnit>> m_TextureUnits;
			std::unique_ptr<Pipeline> m_Pipeline;
			
			
			virtual void GetUniform() = 0;
		public:

			Material() //:Ka(1.0f), Kd(1.0f), Ks(1.0f), exp(1, 1, 1)
			{

			}
			virtual ~Material() {};

			
			virtual void Apply(RenderDevice* renderer,const MatrixParam& matrixParam, const MaterialData& matData = MaterialData()) = 0;
			virtual MaterialType GetType() = 0;
			virtual std::shared_ptr<Material> Clone() = 0;
			virtual void SetPipeline(Pipeline* pipeline) = 0;
			virtual Pipeline* GetPipeline() = 0;
			virtual void AddTexUnit(std::string name,render::TextureUnit unit) = 0;
			virtual void ClearTextureData() = 0;
			
		};
	}
}


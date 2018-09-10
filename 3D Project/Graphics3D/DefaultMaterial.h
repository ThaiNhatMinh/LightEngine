#pragma once

#include "Material.h"
#include <memory>
#include <map>
namespace Light
{
	namespace render
	{
		// pos+normal+uv
		class DefaultMaterial : public Material
		{
		public:
			DefaultMaterial(IContext* pContext);
			//virtual bool VSerialize(IContext*pContext,const tinyxml2::XMLElement * pData);
			//virtual tinyxml2::XMLElement* VDeserialize(tinyxml2::XMLDocument*p);
			virtual void Apply(RenderDevice* renderer, const float* model, const float* mvp);
			virtual MaterialType GetType();
			virtual void SetPipeline(Pipeline* pipeline)override;

			virtual std::shared_ptr<Material> Clone()override;
		private:
			void GetUniform();
			DefaultMaterial() = default; // using for clone 
		private:
			PipelineParam* m_ModelUniform;
			PipelineParam* m_MVPUniform;
			
			std::pair<std::string, std::string> m_ShaderName;
		};
	}

}
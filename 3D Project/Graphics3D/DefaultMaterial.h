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
			DefaultMaterial();
			virtual bool VSerialize(IContext*pContext,const tinyxml2::XMLElement * pData);
			virtual tinyxml2::XMLElement* VDeserialize(tinyxml2::XMLDocument*p);
			virtual void Apply(IActor* pActor);
			virtual void ApplyMatrix(float* model, float* mvp);
			virtual MaterialType GetType();
		private:
			void GetUniform();
		private:
			PipelineParam* m_ModelUniform;
			PipelineParam* m_MVPUniform;
			std::pair<std::string, std::string> m_ShaderName;
		};
	}

}
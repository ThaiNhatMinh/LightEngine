#pragma once

#include "Material.h"
#include "LightManager.h"

#include "..\Interface\IEvent.h"

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
			virtual void Apply(RenderDevice* renderer, const float* model, const float* mvp, const MaterialData& matData);
			virtual MaterialType GetType();
			virtual void SetPipeline(Pipeline* pipeline)override;

			virtual std::shared_ptr<Material> Clone()override;
		private:
			void GetUniform();
			DefaultMaterial() = default; // using for clone 

			void OnSceneCreate(std::shared_ptr<IEvent> event);
		private:
			PipelineParam * m_pModelUniform;
			PipelineParam * m_pMVPUniform;
			PipelineParam* m_uAmbient;
			PipelineParam* m_uDiffuse;
			PipelineParam* m_uSpecular;
			PipelineParam* m_uKa;
			PipelineParam* m_uKd;
			PipelineParam* m_uKs;
			PipelineParam* m_uShiness;
			PipelineParam* m_uCubeTex;
			PipelineParam* m_uCameraPos;
			PipelineParam* m_uNumLight;
			LightParam m_uDirLight;
			std::vector<LightParam> m_uPointLight;
			std::pair<std::string, std::string> m_ShaderName;
			LightManager* m_pLightManager;
		};
	}

}
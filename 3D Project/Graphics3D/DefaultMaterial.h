#pragma once

#include "BaseMaterial.h"
#include "LightManager.h"

#include "..\Interface\IEvent.h"

#include <memory>
#include <map>
namespace Light
{
	namespace render
	{
		
		class DefaultMaterial : public BaseMaterial
		{
		public:
			DefaultMaterial(IContext* pContext);
			virtual void Apply(RenderDevice* renderer, const MatrixParam& matrixParam, const MaterialData& matData)override;
			
			virtual std::shared_ptr<Material> Clone()override;
			
		private:
			void GetUniform();
			DefaultMaterial(); // using for clone 

			void OnSceneCreate(std::shared_ptr<IEvent> event);
		private:
			IContext* m_pContext;
			PipelineParam * m_pModelUniform;
			PipelineParam * m_pMVPUniform;
			PipelineParam* m_uKa;
			PipelineParam* m_uKd;
			PipelineParam* m_uKs;
			PipelineParam* m_uShiness;
			PipelineParam* m_uCameraPos;
			PipelineParam* m_uNumLight;
			LightParam m_uDirLight;
			std::vector<LightParam> m_uPointLight;
			std::pair<std::string, std::string> m_ShaderName;
			LightManager* m_pLightManager;
		};
	}

}
#pragma once

#include "Material.h"
#include "..\Interface\IEvent.h"
#include "LightManager.h"
namespace Light
{
	namespace render
	{
		class SkeletonMaterial : public Material
		{
		public:
			SkeletonMaterial(IContext* pContext);
			virtual void Apply(RenderDevice* renderer, const MatrixParam& matrixParam, const MaterialData& matData)override;
			virtual MaterialType GetType();
			virtual void SetPipeline(Pipeline* pipeline)override;
			virtual std::shared_ptr<Material> Clone()override;
		private:
			SkeletonMaterial()=default; // for clone
			void GetUniform();
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
			LightManager* m_pLightManager;

			PipelineParam* m_uSkeTransform;
		};
	}
}
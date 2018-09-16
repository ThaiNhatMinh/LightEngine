#pragma once

#include "Material.h"

namespace Light
{
	namespace render
	{
		class SkeletonMaterial : public Material
		{
		public:
			SkeletonMaterial(IContext* pContext);
			virtual void Apply(RenderDevice* renderer, const float* model, const float* mvp, const MaterialData& matData);
			virtual MaterialType GetType();
			virtual void SetPipeline(Pipeline* pipeline)override;
			virtual std::shared_ptr<Material> Clone()override;
		private:
			SkeletonMaterial()=default; // for clone
			void GetUniform();
		private:
			PipelineParam * m_pModelUniform;
			PipelineParam * m_pMVPUniform;
			PipelineParam* m_uTex;
			PipelineParam* m_uCubeTex;
			PipelineParam* m_uCameraPos;
			LightParam m_uDirLight;
		};
	}
}
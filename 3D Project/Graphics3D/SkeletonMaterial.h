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
			virtual void Apply(RenderDevice* renderer, const float* model, const float* mvp);
			virtual MaterialType GetType();
			virtual void SetPipeline(Pipeline* pipeline)override;
		private:
			void GetUniform();
		private:
			PipelineParam * m_pModelUniform;
			PipelineParam * m_pMVPUniform;
		};
	}
}
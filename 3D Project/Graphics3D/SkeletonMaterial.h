#pragma once

#include "Material.h"

namespace Light
{
	namespace render
	{
		class SkeletonMaterial : public Material
		{
		public:
			virtual bool VSerialize(IContext*pContext,const tinyxml2::XMLElement * pData);
			virtual tinyxml2::XMLElement* VDeserialize(tinyxml2::XMLDocument*p);
			virtual void Apply(IActor* pActor);
			virtual void ApplyMatrix(float* model, float* mvp);
			virtual MaterialType GetType();
		};
	}
}
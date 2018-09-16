#pragma once
#include "..\Utilities\Utility.h"
#include "Light/DirectionLight.h"
#include "Light/PointLight.h"

namespace Light
{
	namespace render
	{
		struct LightParam;
		class LightManager: public util::Serialization
		{
		public:
			virtual bool VSerialize(IContext*pContext, const tinyxml2::XMLElement * pData) override;
			virtual tinyxml2::XMLElement* VDeserialize(tinyxml2::XMLDocument*p)override;

			void	SetupDirLight(LightParam*);
			void	SetupPointLight(std::vector<LightParam>&);
			int		GetNumPointLight();

		private:
			DirectionLight m_DirLight;
			std::vector<PointLight> m_PointLights;
		};
	}
}
#include "pch.h"
#include "DirectionLight.h"
#include "..\Material.h"

namespace Light
{
	namespace render
	{
		void DirectionLight::SetupParam(LightParam * param)
		{
			(*param)["Ia"]->SetAsVec3(glm::value_ptr(Ia));
			(*param)["Id"]->SetAsVec3(glm::value_ptr(Id));
			(*param)["Is"]->SetAsVec3(glm::value_ptr(Is));
			(*param)["Direction"]->SetAsVec3(glm::value_ptr(direction));
		}

	}
}	
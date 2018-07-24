#pragma once
#include "..\Interface\IComponent.h"


namespace Light
{
	class TransformComponent : public ITransformComponent
	{
	public:

		virtual bool VSerialize(const tinyxml2::XMLElement* pData);
		virtual tinyxml2::XMLElement* VDeserialize(tinyxml2::XMLDocument*p) { return nullptr; }
	};
}
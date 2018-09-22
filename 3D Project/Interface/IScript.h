#pragma once
#include "..\Utilities\Utility.h"

namespace Light
{
	// Using attach to Actor
	// Make Actor can do logic
	// Just test, can using for Lua script or JS
	class IScript: public util::Serialization
	{
	protected:
		//IActor * m_pOwner;

	public:
		virtual ~IScript() = default;
		virtual void PostInit() = 0;
		virtual void Update(float dt) = 0;
	};

	// Script do nothing
	class NullScript : public IScript
	{
	public:
		virtual void PostInit() {};
		virtual void Update(float dt) {};
		virtual bool VSerialize(IContext*pContext, const tinyxml2::XMLElement * pData) { return 1; }
		virtual tinyxml2::XMLElement* VDeserialize(tinyxml2::XMLDocument*p) { return nullptr; }
	};
}
#pragma once

#include "..\Interface\IScript.h"
#include "..\Interface\IComponent.h"
#include "..\Interface\IInput.h"

namespace Light
{
	class TestScript: public IScript
	{
	private:
		IInput* m_pInput;
		IAnimatorComponent* m_pAnimator;
	public:
		TestScript(IContext * pContext, IActor * owner);
		virtual bool VSerialize(IContext*pContext, const tinyxml2::XMLElement * pData);
		virtual tinyxml2::XMLElement* VDeserialize(tinyxml2::XMLDocument*p);
		virtual void PostInit() {};
		virtual void Update(float dt);
	};
}
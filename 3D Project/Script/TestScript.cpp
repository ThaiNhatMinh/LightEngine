#include "pch.h"
#include "TestScript.h"

#include "..\Interface\IActor.h"
Light::TestScript::TestScript(IContext * pContext, IActor * owner)
{
	m_pInput = pContext->GetSystem<IInput>();
	m_pAnimator = owner->GetComponent<IAnimatorComponent>();

}

bool Light::TestScript::VSerialize(IContext * pContext, const tinyxml2::XMLElement * pData)
{
	

	return true;
}

tinyxml2::XMLElement * Light::TestScript::VDeserialize(tinyxml2::XMLDocument * p)
{
	return nullptr;
}

void Light::TestScript::Update(float dt)
{
	if (m_pInput->VOnKey(Key::Space)) 
		m_pAnimator->Play("Reload", 0, 0);
}

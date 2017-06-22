#include "pch.h"
#include "CharacterControllerComponent.h"

const char* CharacterControllerComponent::Name = "CharacterControllerComponent";


CharacterControllerComponent::CharacterControllerComponent():m_fJumpForce(0),m_fMaxSpeed(0),m_Direction(0,0,1),m_JumpDirection(0,1,0)
{
}

CharacterControllerComponent::~CharacterControllerComponent()
{

}

bool CharacterControllerComponent::VInit(tinyxml2::XMLElement * pData)
{
	tinyxml2::XMLElement* pSpeedElement = pData->FirstChildElement("Speed");
	m_fMaxSpeed = pSpeedElement->DoubleAttribute("max", 1.0f);

	tinyxml2::XMLElement* pJumpElement = pData->FirstChildElement("Jump");
	m_fJumpForce = pJumpElement->DoubleAttribute("force", 1.0f);

	return true;

}

void CharacterControllerComponent::VPostInit(void)
{
	
}

void CharacterControllerComponent::VUpdate(float dt)
{
}

void CharacterControllerComponent::VOnChanged(void)
{
}

const char * CharacterControllerComponent::VGetName() const
{
	return Name;
}

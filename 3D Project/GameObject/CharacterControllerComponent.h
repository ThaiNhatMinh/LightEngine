#pragma once
#include "..\pch.h"

// WASD controller for player
class CharacterControllerComponent : public ActorComponent
{
private:
	vec3			m_Direction;
	float			m_fMaxSpeed;
	float			m_fJumpForce;
	vec3			m_JumpDirection;
	btRigidBody*	m_pRB;
public:
	CharacterControllerComponent();
	~CharacterControllerComponent();
	static const char*	Name;

	virtual bool VInit(tinyxml2::XMLElement* pData);
	virtual void VPostInit(void);
	virtual void VUpdate(float dt);
	virtual void VOnChanged(void);

	virtual const char *VGetName() const ;
};
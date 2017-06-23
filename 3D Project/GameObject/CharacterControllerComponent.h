#pragma once
#include "..\pch.h"

enum ObjState
{
	STAND = 1,
	JUMPING = 2,
	FALLING = 4,
	WALKING = 8
};
// WASD controller for player
class CharacterControllerComponent : public ActorComponent
{
private:
	vec3			m_Direction;
	float			m_fMaxSpeed;
	float			m_fJumpForce;
	vec3			m_JumpDirection;
	btRigidBody*	m_pRB;
	bool			m_bCanJump;
	TransformComponent *m_pTransformC;
public:
	CharacterControllerComponent();
	~CharacterControllerComponent();
	static const char*	Name;

	virtual bool VInit(tinyxml2::XMLElement* pData);
	virtual tinyxml2::XMLElement* VGenerateXml(tinyxml2::XMLDocument*p);
	virtual void VPostInit(void);
	virtual void VUpdate(float dt);
	virtual void VOnChanged(void);

	virtual const char *VGetName() const ;

	// Event
	void PhysicCollisionEvent(const IEvent* pEvent);
};
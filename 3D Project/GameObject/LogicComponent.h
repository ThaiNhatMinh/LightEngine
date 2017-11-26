#pragma once
//#include "e:\Code Collection\3D Project\3D Project\GameObject\ActorComponent.h"
class LogicComponent :public ActorComponent
{
private:
	vec3 m_Position; // position of camera
	vec3 m_Front;
	vec3 m_Right;
	vec3 WorldUp;
	vec3 m_Up;
	float m_Speed;
	float m_Pitch;	// x axis
	float m_Yaw;	// y axis
	float MouseSensitivity;
	TransformComponent* m_TF;
public:
	static const char*	Name;
	LogicComponent();
	~LogicComponent();
	virtual bool VInit(const tinyxml2::XMLElement* pData) { return 1; };
	virtual tinyxml2::XMLElement* VGenerateXml(tinyxml2::XMLDocument*p);
	virtual const char *VGetName() const ;
	virtual void VPostInit(void);
	virtual void VUpdate(float timeStep);
	virtual void FixedUpdate(float timeStep);
	virtual void FixedPostUpdate(float timeStep);
private:
	// Event
	void PhysicPreStepEvent(std::shared_ptr<const IEvent> pEvent);
	void PhysicPostStepEvent(std::shared_ptr<const IEvent> pEvent);
};


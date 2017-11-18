#include "pch.h"



// all times are units-per-second
const float DEFAULT_MAX_VELOCITY = 7.5f;
const float DEFAULT_MAX_ANGULAR_VELOCITY = 1.2f;


//---------------------------------------------------------------------------------------------------------------------
// PhysicsComponent implementation
//---------------------------------------------------------------------------------------------------------------------

const char *PhysicsComponent::Name = "PhysicsComponent";

PhysicsComponent::PhysicsComponent(void)
{
	m_RigidBodyLocation = vec3(0.f, 0.f, 0.f);
	m_RigidBodyOrientation = vec3(0.f, 0.f, 0.f);
	m_RigidBodyScale = vec3(1.f, 1.f, 1.f);

	m_acceleration = 0;
	m_angularAcceleration = 0;
	m_maxVelocity = DEFAULT_MAX_VELOCITY;
	m_maxAngularVelocity = DEFAULT_MAX_ANGULAR_VELOCITY;
}

PhysicsComponent::~PhysicsComponent(void)
{
	gPhysic()->VRemoveActor(m_pOwner->GetId());
}

bool PhysicsComponent::VInit(tinyxml2::XMLElement* pData)
{

	// shape
	tinyxml2::XMLElement* pShape = pData->FirstChildElement("Shape");
	if (pShape)
	{
		m_shape = pShape->FirstChild()->Value();
	}

	// density
	tinyxml2::XMLElement* pDensity = pData->FirstChildElement("Density");
	if (pDensity)
		m_density = pDensity->FirstChild()->Value();

	// material
	tinyxml2::XMLElement* pMaterial = pData->FirstChildElement("PhysicsMaterial");
	if (pMaterial)
		m_material = pMaterial->FirstChild()->Value();

	// initial transform
	tinyxml2::XMLElement* pRigidBodyTransform = pData->FirstChildElement("RigidBodyTransform");
	if (pRigidBodyTransform)
		BuildRigidBodyTransform(pRigidBodyTransform);

	return true;
}

tinyxml2::XMLElement* PhysicsComponent::VGenerateXml(tinyxml2::XMLDocument* doc)
{
	tinyxml2::XMLElement* pBaseElement = doc->NewElement(VGetName());

	// shape
	tinyxml2::XMLElement* pShape = doc->NewElement("Shape");
	tinyxml2::XMLText* pShapeText = doc->NewText(m_shape.c_str());
	pShape->LinkEndChild(pShapeText);
	pBaseElement->LinkEndChild(pShape);

	// density
	tinyxml2::XMLElement* pDensity = doc->NewElement("Density");
	tinyxml2::XMLText* pDensityText = doc->NewText(m_density.c_str());
	pDensity->LinkEndChild(pDensityText);
	pBaseElement->LinkEndChild(pDensity);

	// material
	tinyxml2::XMLElement* pMaterial = doc->NewElement("Material");
	tinyxml2::XMLText* pMaterialText = doc->NewText(m_material.c_str());
	pMaterial->LinkEndChild(pMaterialText);
	pBaseElement->LinkEndChild(pMaterial);

	// rigid body transform
	tinyxml2::XMLElement* pInitialTransform = doc->NewElement("RigidBodyTransform");

	// initial transform -> position
	tinyxml2::XMLElement* pPosition = doc->NewElement("Position");
	pPosition->SetAttribute("x", ToStr(m_RigidBodyLocation.x).c_str());
	pPosition->SetAttribute("y", ToStr(m_RigidBodyLocation.y).c_str());
	pPosition->SetAttribute("z", ToStr(m_RigidBodyLocation.z).c_str());
	pInitialTransform->LinkEndChild(pPosition);

	// initial transform -> orientation
	tinyxml2::XMLElement* pOrientation = doc->NewElement("Orientation");
	pOrientation->SetAttribute("yaw", ToStr(m_RigidBodyOrientation.x).c_str());
	pOrientation->SetAttribute("pitch", ToStr(m_RigidBodyOrientation.y).c_str());
	pOrientation->SetAttribute("roll", ToStr(m_RigidBodyOrientation.z).c_str());
	pInitialTransform->LinkEndChild(pOrientation);

	// initial transform -> scale 
	tinyxml2::XMLElement* pScale = doc->NewElement("Scale");
	pScale->SetAttribute("x", ToStr(m_RigidBodyScale.x).c_str());
	pScale->SetAttribute("y", ToStr(m_RigidBodyScale.y).c_str());
	pScale->SetAttribute("z", ToStr(m_RigidBodyScale.z).c_str());
	pInitialTransform->LinkEndChild(pScale);

	pBaseElement->LinkEndChild(pInitialTransform);

	return pBaseElement;
}

void PhysicsComponent::VPostInit(void)
{
	if (m_pOwner)
	{
		if (m_shape == "Sphere")
		{
//			gPhysic()->VAddSphere((float)m_RigidBodyScale.x, m_pOwner, m_density, m_material);
		}
		else if (m_shape == "Box")
		{
//			gPhysic()->VAddBox(m_RigidBodyScale, m_pOwner, m_density, m_material);
		}
		else if (m_shape == "PointCloud")
		{
			E_ERROR("Not supported yet!");
		}
	}
}

void PhysicsComponent::VUpdate(float deltaMs)
{
	// get the transform component
	TransformComponent* pTransformComponent = m_pOwner->GetComponent<TransformComponent>(TransformComponent::Name);
	if (!pTransformComponent)
	{
		E_ERROR("No transform component!");
		return;
	}

	// get the direction the object is facing
	mat4 transform = pTransformComponent->GetTransform();

	if (m_acceleration != 0)
	{
		// calculate the acceleration this frame

		float accelerationToApplyThisFrame = m_acceleration / 1000.f * (float)deltaMs;

		// Get the current velocity vector and convert to a scalar.  The velocity vector is a combination of 
		// the direction this actor is going in and the speed of the actor.  The scalar is just the speed 
		// component.
		//vec3 velocity(gPhysic()->VGetVelocity(m_pOwner->GetId()));
		//float velocityScalar = glm::length(velocity);

		//vec3 direction(Math::GetAxis(transform,Z_AXIS));
		//gPhysic()->VApplyForce(direction, accelerationToApplyThisFrame, m_pOwner->GetId());

		// logging
		// [rez] Comment this back in if you want to debug the physics thrust & rotation stuff.  It spams quite 
		// a bit of info the output window so I'm commenting it out for now.
		/*
		GCC_LOG("Actor", "Acceleration: " + ToStr(accelerationToApplyThisFrame) + "; velocityScalar: " + ToStr(velocityScalar) + \
		"; direction: " + ToStr(direction) + "; direction.Length(): " + ToStr(direction.Length()) + \
		"; velocity: " + ToStr(velocity) + "; velocity.Length(): " + ToStr(velocity.Length()));
		*/
	}

	if (m_angularAcceleration != 0)
	{
		// calculate the acceleration this frame
		float angularAccelerationToApplyThisFrame = m_angularAcceleration / 1000.f * (float)deltaMs;
		//gPhysic()->VApplyTorque(Math::GetAxis(transform,Y_AXIS), angularAccelerationToApplyThisFrame, m_pOwner->GetId());

		// logging
		// [rez] Comment this back in if you want to debug the physics thrust & rotation stuff.  It spams quite 
		// a bit of info the output window so I'm commenting it out for now.
		//GCC_LOG("Actor", "Angular Acceleration: " + ToStr(angularAccelerationToApplyThisFrame) );
	}

	//pTransformComponent->SetTransform(gPhysic()->VGetTransform(m_pOwner->GetId()));

}

void PhysicsComponent::BuildRigidBodyTransform(tinyxml2::XMLElement* pTransformElement)
{
	// FUTURE WORK Mrmike - this should be exactly the same as the TransformComponent - maybe factor into a helper method?

	tinyxml2::XMLElement* pPositionElement = pTransformElement->FirstChildElement("Position");
	if (pPositionElement)
	{
		double x = pPositionElement->DoubleAttribute("x");
		double y = pPositionElement->DoubleAttribute("y");
		double z = pPositionElement->DoubleAttribute("z");
		m_RigidBodyLocation = vec3(x, y, z);
	}

	tinyxml2::XMLElement* pOrientationElement = pTransformElement->FirstChildElement("Orientation");
	if (pOrientationElement)
	{
		double yaw = pPositionElement->DoubleAttribute("yaw");
		double pitch = pPositionElement->DoubleAttribute("pitch");
		double roll = pPositionElement->DoubleAttribute("roll");
		
		
		m_RigidBodyOrientation = vec3((float)glm::radians(yaw), (float)glm::radians(pitch), (float)glm::radians(roll));
	}

	tinyxml2::XMLElement* pScaleElement = pTransformElement->FirstChildElement("Scale");
	if (pScaleElement)
	{
		double x = pScaleElement->DoubleAttribute("x");
		double y = pScaleElement->DoubleAttribute("y");
		double z = pScaleElement->DoubleAttribute("z");
		m_RigidBodyScale = vec3((float)x, (float)y, (float)z);
	}
}

void PhysicsComponent::ApplyForce(const vec3& direction, float forceNewtons)
{
	//gPhysic()->VApplyForce(direction, forceNewtons, m_pOwner->GetId());
}

void PhysicsComponent::ApplyTorque(const vec3& direction, float forceNewtons)
{
	//gPhysic()->VApplyTorque(direction, forceNewtons, m_pOwner->GetId());
}

bool PhysicsComponent::KinematicMove(const mat4 &transform)
{
	return 1;
	//return gPhysic()->VKinematicMove(transform, m_pOwner->GetId());
}

void PhysicsComponent::ApplyAcceleration(float acceleration)
{
	m_acceleration = acceleration;
}

void PhysicsComponent::RemoveAcceleration(void)
{
	m_acceleration = 0;
}

void PhysicsComponent::ApplyAngularAcceleration(float acceleration)
{
	m_angularAcceleration = acceleration;
}

void PhysicsComponent::RemoveAngularAcceleration(void)
{
	m_angularAcceleration = 0;
}

vec3 PhysicsComponent::GetVelocity(void)
{
//	return gPhysic()->VGetVelocity(m_pOwner->GetId());
	return vec3();
}

void PhysicsComponent::SetVelocity(const vec3& velocity)
{
//	gPhysic()->VSetVelocity(m_pOwner->GetId(), velocity);
}

void PhysicsComponent::RotateY(float angleRadians)
{
	TransformComponent* pTransformComponent = m_pOwner->GetComponent<TransformComponent>(TransformComponent::Name);
	if (pTransformComponent)
	{
		mat4 transform = pTransformComponent->GetTransform();
		vec3 position = glm::vec3(transform[3]);

		
		quat q;
		q = glm::rotate(q, glm::degrees(angleRadians), vec3(0.0f, 1.0f, 0.0f));
		
		mat4 rotateY = glm::toMat4(q);
		rotateY = glm::translate(rotateY, position);

		KinematicMove(rotateY);
	}
	else
		E_ERROR("Attempting to call RotateY() on actor with no transform component");
}

void PhysicsComponent::SetPosition(float x, float y, float z)
{
	TransformComponent* pTransformComponent = m_pOwner->GetComponent<TransformComponent>(TransformComponent::Name);
	if (pTransformComponent)
	{
		mat4 transform = pTransformComponent->GetTransform();
		vec3 position = vec3(x, y, z);
		transform = glm::translate(transform,position);

		KinematicMove(transform);
	}
	else
		E_ERROR("Attempting to call RotateY() on actor with no trnasform component");
}

void PhysicsComponent::Stop(void)
{
	//return gPhysic()->VStopActor(m_pOwner->GetId());
}
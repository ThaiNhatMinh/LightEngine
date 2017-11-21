#include "pch.h"

const char* ColliderComponent::Name = "ColliderComponent";

ColliderComponent::ColliderComponent(void):m_pCollisionShape(nullptr)
{
}

ColliderComponent::~ColliderComponent(void)
{
}

tinyxml2::XMLElement * ColliderComponent::VGenerateXml(tinyxml2::XMLDocument * doc)
{
	return nullptr;
}

bool ColliderComponent::VInit(const tinyxml2::XMLElement* pData)
{
	if (!pData) return false;
	// shape
	const tinyxml2::XMLElement* pShape = pData->FirstChildElement("Shape");
	if (pShape == nullptr) return false;
	m_ShapeName = pShape->FirstChild()->Value();

	CreateShape(m_ShapeName,pData);

	return true;
}

btCollisionShape * ColliderComponent::GetCollisionShape()
{
	return m_pCollisionShape;
}

void ColliderComponent::CreateShape(string name, const tinyxml2::XMLElement* pData)
{
	if (name == "Box")
	{
		// size
		const tinyxml2::XMLElement* pSize = pData->FirstChildElement("HalfSize");
		if (pSize == nullptr) return;
		vec3 v;
		v.x = pSize->DoubleAttribute("x", 1.0f);
		v.y = pSize->DoubleAttribute("y", 1.0f);
		v.z = pSize->DoubleAttribute("z", 1.0f);

		m_pCollisionShape = new btBoxShape(ToBtVector3(v));
		return;
	}

	if (name == "Sphere")
	{
		// size
		const tinyxml2::XMLElement* pSize = pData->FirstChildElement("Radius");
		if (pSize == nullptr) return;
		float r = pSize->DoubleAttribute("r", 1.0f);
		m_pCollisionShape = new btSphereShape(r);
		return;
	}
}

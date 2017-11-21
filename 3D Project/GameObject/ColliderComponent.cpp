#include "pch.h"
#include <BulletCollision\CollisionShapes\btHeightfieldTerrainShape.h>

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

ShapeType ColliderComponent::GetType()
{
	return m_Type;
}

vec2 ColliderComponent::GetMinMax()
{
	return m_MM;
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
	}else if (name == "Sphere")
	{
		// size
		const tinyxml2::XMLElement* pSize = pData->FirstChildElement("Radius");
		if (pSize == nullptr) return;
		float r = pSize->DoubleAttribute("r", 1.0f);
		m_pCollisionShape = new btSphereShape(r);
		return;
	}
	else if (name == "HeightMap")
	{
		const tinyxml2::XMLElement* pFile = pData->FirstChildElement("File");
		if (pFile == nullptr) return;
		const char* path = pFile->Attribute("Path");
		if (path == nullptr) return;
		HeightMap hm = gResources()->LoadHeightMap(path);
		m_pCollisionShape = new btHeightfieldTerrainShape(hm.Width, hm.Height, hm.Data, 1.0f, hm.minH, hm.maxH, 1, PHY_UCHAR, false);
		btVector3 localScaling(hm.stepsize, 1.0, hm.stepsize);
		m_pCollisionShape->setLocalScaling(localScaling);
		m_MM = vec2(hm.minH, hm.maxH);
		m_Type = SHAPE_TERRAIN;
	}
}

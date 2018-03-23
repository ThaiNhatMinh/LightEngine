#pragma once

#include <pch.h>
#include "ActorComponent.h"
/// Collision shape type.
enum ShapeType
{
	SHAPE_NONE,
	SHAPE_BOX ,
	SHAPE_SPHERE,
	SHAPE_CAPSULE,
	SHAPE_CYLINDER,
	SHAPE_STATICPLANE,
	SHAPE_CONE,
	SHAPE_TRIANGLEMESH,
	SHAPE_CONVEXHULL,
	SHAPE_TERRAIN,
	SHAPE_CHARACTER
};

const char ShapeName[][MAX_NAME] = { "Box","Sphere","Capsule","Cylinder" };

class TriangleMesh : public btBvhTriangleMeshShape
{
public:
	TriangleMesh(btStridingMeshInterface *meshInterface, bool useQuantizedAabbCompression, bool buildBvh = true);
	~TriangleMesh();
	void SetInfoMap(btTriangleInfoMap* p) { m_pInfoMap = std::unique_ptr<btTriangleInfoMap>(p); }
private:
	std::unique_ptr<btStridingMeshInterface>	m_pMeshInterface;
	std::unique_ptr<btTriangleInfoMap>			m_pInfoMap;
};

class ColliderComponent : public ActorComponent
{
public:
	const static char *Name;
	virtual const char *VGetName() const override { return ColliderComponent::Name; }

public:
	ColliderComponent(void);
	virtual ~ColliderComponent(void);
	virtual tinyxml2::XMLElement* VGenerateXml(tinyxml2::XMLDocument * doc) override;

	// ActorComponent interface
	virtual bool		VInit(const tinyxml2::XMLElement* pData) override;

	// Component interface
	btCollisionShape*	GetCollisionShape();
	void				SetCollisionShape(btCollisionShape* shape);
	ShapeType			GetType();
	// for heightmap
	vec2				GetMinMax();
	
protected:
	void CreateShape(string name, const tinyxml2::XMLElement* pData);

private:
	vec2				m_MM;
	ShapeType			m_Type;
	btCollisionShape*	m_pCollisionShape;
	string				m_ShapeName;
};
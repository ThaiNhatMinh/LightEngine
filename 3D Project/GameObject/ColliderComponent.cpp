#include "pch.h"
#include <BulletCollision\CollisionShapes\btHeightfieldTerrainShape.h>
#include <BulletCollision\CollisionDispatch\btInternalEdgeUtility.h>

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

void ColliderComponent::SetCollisionShape(btCollisionShape * shape)
{
	m_pCollisionShape = shape;
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
		
		m_Type = SHAPE_BOX;
		return;
	}else if (name == "Sphere")
	{
		// size
		const tinyxml2::XMLElement* pSize = pData->FirstChildElement("Radius");
		if (pSize == nullptr) return;
		float r = pSize->DoubleAttribute("r", 1.0f);
		m_pCollisionShape = new btSphereShape(r);
		m_Type = SHAPE_SPHERE;
		return;
	}
	else if (name == "Character")
	{
		AnimationComponent* pAnim = m_pOwner->GetComponent<AnimationComponent>(AnimationComponent::Name);
		AABB aabb = pAnim->GetUserDimesion();
		m_pCollisionShape = new btBoxShape(ToBtVector3(aabb.Max));

		m_Type = SHAPE_CHARACTER;
	}
	else if (name == "HeightMap")
	{
		const tinyxml2::XMLElement* pFile = pData->FirstChildElement("File");
		if (pFile == nullptr) return;
		const char* path = pFile->Attribute("Path");
		if (path == nullptr) return;
		HeightMap* hm = gResources()->GetHeightMap(path);
		m_pCollisionShape = new btHeightfieldTerrainShape(hm->Width, hm->Height, hm->Data, 1, hm->minH, hm->maxH, 1, PHY_UCHAR, false);
		btVector3 localScaling(hm->stepsize, hm->hscale, hm->stepsize);
		m_pCollisionShape->setLocalScaling(localScaling);
		m_MM = vec2(hm->minH, hm->maxH);
		m_Type = SHAPE_TERRAIN;
	}
	else if (name == "TriangleMesh")
	{
		const tinyxml2::XMLElement* pFile = pData->FirstChildElement("File");
		if (pFile == nullptr) return;
		const char* path = pFile->Attribute("Path");
		if (path == nullptr) return;

		HeightMap* hm = gResources()->GetHeightMap(path);
		
		Mesh* m = static_cast<Mesh*>(hm->m_Mesh[0].get());
		int totaltriangle = m->m_Indices.size() / 3;
		int totalVerts = m->m_Vertexs.size();
		int indexStride = sizeof(unsigned int) * 3;
		int vertStride = sizeof(DefaultVertex);
		btTriangleIndexVertexArray* pIntexVertexArray = new btTriangleIndexVertexArray(totaltriangle, (int*)&m->m_Indices[0], indexStride, totalVerts, &m->m_Vertexs[0].pos.x, vertStride);
		TriangleMesh* pShape = new TriangleMesh(pIntexVertexArray, true, true);
		btTriangleInfoMap* triangleInfoMap = new btTriangleInfoMap();

		btGenerateInternalEdgeInfo(pShape, triangleInfoMap);
		pShape->SetInfoMap(triangleInfoMap);
		m_pCollisionShape = pShape;
		m_Type = SHAPE_TRIANGLEMESH;
	}
}

TriangleMesh::TriangleMesh(btStridingMeshInterface *meshInterface, bool useQuantizedAabbCompression, bool buildBvh): btBvhTriangleMeshShape(meshInterface,useQuantizedAabbCompression,buildBvh),m_pMeshInterface(meshInterface)
{

}

TriangleMesh::~TriangleMesh()
{
	
}

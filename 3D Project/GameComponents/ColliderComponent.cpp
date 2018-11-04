#include <pch.h>
#include <BulletCollision\CollisionShapes\btHeightfieldTerrainShape.h>
#include <BulletCollision\CollisionDispatch\btInternalEdgeUtility.h>

#include "..\Utilities\PhysicsUtilities.h"
#include "..\Interface\IActor.h"
#include "..\Interface\IResourceManager.h"

#include "AnimatorComponent.h"

#include "collidercomponent.h"
#include "..\Math\Math.h"
namespace Light
{
namespace physics
{


ColliderComponent::ColliderComponent(void):m_pCollisionShape(nullptr)
{
}

ColliderComponent::~ColliderComponent(void)
{
}

tinyxml2::XMLElement * ColliderComponent::VDeserialize(tinyxml2::XMLDocument*p)
{
	return nullptr;
}

bool ColliderComponent::VSerialize(IContext* pContex,const tinyxml2::XMLElement* pData)
{
	if (!pData) return false;
	// shape
	const tinyxml2::XMLElement* pShape = pData->FirstChildElement("Shape");
	if (pShape == nullptr) return false;
	m_ShapeName = pShape->FirstChild()->Value();

	CreateShape(m_ShapeName,pData, pContex);

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

ShapeType ColliderComponent::GetShapeType()
{
	return m_Type;
}

vec2 ColliderComponent::GetMinMax()
{
	return m_MM;
}

void ColliderComponent::CreateShape(string name, const tinyxml2::XMLElement* pData, IContext* pContex)
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
		IAnimatorComponent* pAnim = m_pOwner->GetComponent<IAnimatorComponent>();
		math::AABB aabb;// = pAnim->GetUserDimesion();
		//m_pCollisionShape = new btBoxShape(ToBtVector3(aabb.Max));
		// r = aabb.Max.x
		m_pCollisionShape = new btCapsuleShape(aabb.Max.x, aabb.Max.y*2 - aabb.Max.x*2);
		//m_pCollisionShape->setMargin(0);
		m_Type = SHAPE_CHARACTER;
	}
	/*else if (name == "HeightMap")
	{
		const tinyxml2::XMLElement* pFile = pData->FirstChildElement("File");
		if (pFile == nullptr) return;
		const char* path = pFile->Attribute("Path");
		if (path == nullptr) return;
		resources::HeightMap* hm = pContex->GetSystem<resources::IResourceManager>()->VGetHeightMap(path);
		m_pCollisionShape = new btHeightfieldTerrainShape(hm->Width, hm->Height, hm->Data.get(), 1, hm->minH, hm->maxH, 1, PHY_UCHAR, false);
		btVector3 localScaling(hm->stepsize, hm->hscale, hm->stepsize);
		m_pCollisionShape->setLocalScaling(localScaling);
		m_MM = vec2(hm->minH, hm->maxH);
		m_Type = SHAPE_TERRAIN;
	}*/
	else if (name == "TriangleMesh")
	{
		auto pParam = pData->FirstChildElement("Param");
		int stepsize = pParam->Int64Attribute("StepSize", 50);
		int hscale = pParam->Int64Attribute("HScale", 10);
		int numSub = pParam->Int64Attribute("NumSub", 2);

		const tinyxml2::XMLElement* pFile = pData->FirstChildElement("File");
		if (pFile == nullptr) return;
		const char* path = pFile->Attribute("Path");
		if (path == nullptr) return;

		resources::HeightMapData* hm = pContex->GetSystem<resources::IResourceManager>()->VGetHeightMap(path);
		
		auto vertexs = math::GenerateVertexData(hm, stepsize, hm->Width, hm->Height, hscale, numSub);
		auto indices = math::GenerateIndicesData(hm);
		
		btTriangleIndexVertexArray* pIntexVertexArray = new TriangleData(vertexs,indices);
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
TriangleData::TriangleData(std::vector<DefaultVertex> vertexs, std::vector<unsigned int> indices):vertexs(vertexs),indices(indices)
{
	int totaltriangle = indices.size() / 3;
	int totalVerts = vertexs.size();
	int indexStride = sizeof(unsigned int) * 3;
	int vertStride = sizeof(DefaultVertex);

	btIndexedMesh mesh;

	mesh.m_numTriangles = totaltriangle;
	mesh.m_triangleIndexBase = (const unsigned char *)&(this->indices[0]);
	mesh.m_triangleIndexStride = indexStride;
	mesh.m_numVertices = totalVerts;
	mesh.m_vertexBase = (const unsigned char *)&(this->vertexs[0]);
	mesh.m_vertexStride = vertStride;
	
	addIndexedMesh(mesh);
}
}
}
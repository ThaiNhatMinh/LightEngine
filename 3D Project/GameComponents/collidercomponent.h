#pragma once

#include "..\Interface\IComponent.h"
#include <glm.hpp>

// Bullet Physic 
#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>

namespace Light
{
	namespace physics
	{
		/// Collision shape type.
		

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

		class TriangleData : public btTriangleIndexVertexArray
		{
		public:
			TriangleData(std::vector<glm::vec3> vertexs, std::vector<unsigned int> indices);
		private:
			std::vector<glm::vec3> vertexs;
			std::vector<unsigned int> indices;
		};
		class ColliderComponent : public IColliderComponent
		{
			friend class RigidBodyComponent;

		public:
			ColliderComponent(void);
			virtual ~ColliderComponent(void);
			
			virtual bool VSerialize(IContext*pContext, const tinyxml2::XMLElement * pData);
			virtual tinyxml2::XMLElement* VDeserialize(tinyxml2::XMLDocument*p);

			// Component interface
			btCollisionShape*		GetCollisionShape();
			void					SetCollisionShape(btCollisionShape* shape);
			virtual ShapeType		GetShapeType()override;
			// for heightmap
			glm::vec2				GetMinMax();

		protected:
			void CreateShape(string name, const tinyxml2::XMLElement* pData, IContext* pContex);
			
		private:
			glm::vec2				m_MM;
			ShapeType			m_Type;
			btCollisionShape*	m_pCollisionShape;
			string				m_ShapeName;
		};
	}
}
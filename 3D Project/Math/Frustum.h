#pragma once
#include <glm\mat4x4.hpp>
#include "Plane.h"
namespace Light
{
	namespace math
	{
		class Frustum
		{
		private:
			enum Side { Near, Far, Top, Bottom, Left, Right, NumPlane };
			glm::mat4 m_ProjMatrix;
		public:
			Plane m_Plane[6];			// Plane of frustum in camera space
			glm::vec3 m_NearClip[4];					// 4 point in near clip plane in camera space
			glm::vec3 m_FarClip[4];					// 4 point in far clip plane in camera space

			float m_Fov;						// fied of view in radian
			float m_Aspect;						// aspect ratio  - width / height
			float m_Near;						// Near distance
			float m_Far;						// Far distance
			float tanFovOver2;

			//VertexArray				VAO;
			//BufferObject			VBO, EBO;
		protected:

		public:
			Frustum(float fov, float aspect, float _near, float _far);
			Frustum();
			~Frustum();
			bool Inside(const glm::vec3& v)const;
			bool Inside(const glm::vec3& v, float radius)const;
			bool Inside(const glm::vec3& v, const glm::vec3& v2)const;
			const Plane& Get(Side side) { return m_Plane[side]; }
			void SetFov(float fov) { m_Fov = fov; Frustum(m_Fov, m_Aspect, m_Near, m_Far); }
			void SetAspect(float aspect) { m_Aspect = aspect;  Frustum(m_Fov, m_Aspect, m_Near, m_Far); };
			void SetNear(float _near) { m_Near = _near;  Frustum(m_Fov, m_Aspect, m_Near, m_Far); };
			void SetFar(float _far) { m_Far = _far;  Frustum(m_Fov, m_Aspect, m_Near, m_Far); };
			void Update(const glm::vec3& pos, const glm::vec3& front, const glm::vec3& right);
			const glm::mat4& GetProjMatrix();

		};
	}
}
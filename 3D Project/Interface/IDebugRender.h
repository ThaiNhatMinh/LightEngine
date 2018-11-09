#pragma once
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
namespace Light
{
	class Scene;
	class IDebugRender : public ISubSystem
	{
	public:
		virtual void DrawLine(const glm::vec3& from, const glm::vec3& to, const glm::vec3& color = glm::vec3(1,1,1), const glm::mat4& m = glm::mat4()) = 0;
		virtual void DrawLineBox(glm::vec3 min, glm::vec3 max, glm::vec3 color = glm::vec3(0.5f), const glm::mat4& m = glm::mat4()) = 0;
		virtual void DrawCoord(const glm::mat4& m) = 0;
		virtual void AddLightBox(glm::vec3 pos, glm::vec3 color) = 0;
		virtual void Render() = 0;
	};
}
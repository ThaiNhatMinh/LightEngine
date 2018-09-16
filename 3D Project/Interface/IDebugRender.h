#pragma once

namespace Light
{
	class Scene;
	class IDebugRender : public ISubSystem
	{
	public:
		virtual void DrawLine(const vec3& from, const vec3& to, const vec3& color = glm::vec3(1,1,1), const mat4& m = mat4()) = 0;
		virtual void DrawLineBox(vec3 min, vec3 max, vec3 color = vec3(0.5f), const mat4& m = mat4()) = 0;
		virtual void DrawCoord(const mat4& m) = 0;
		virtual void AddLightBox(glm::vec3 pos, glm::vec3 color) = 0;
		virtual void Render() = 0;
	};
}
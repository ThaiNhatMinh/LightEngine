#pragma once


namespace Light
{
	class OpenGLDebugRender: public IDebugRender
	{
	public:
		OpenGLDebugRender(IContext* pContext);
		//virtual void DrawLine(const vec3& from, const vec3& to, const vec3& color = glm::vec3(1, 1, 1), const mat4& m = mat4());
		//virtual void DrawLineBox(vec3 min, vec3 max, vec3 color = vec3(0.5f), const mat4& m = mat4());
		//virtual void DrawCoord(const mat4& m);

		//virtual void Render(Scene* pScene);
	};
}
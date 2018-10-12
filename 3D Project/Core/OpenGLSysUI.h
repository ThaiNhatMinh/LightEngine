#pragma once

#include "..\Interface\ISysUI.h"
#include "..\Interface\Renderer.h"
#include "..\Interface\ITimer.h"
#include <imgui.h>
#include <glfw3.h>

namespace Light
{
	class OpenGLSysUI: public ISysUI
	{
		friend class OpenGLWindows;
		const static int MaxData = 200;
	private:
		float values[MaxData] = { 0 };
		int values_offset = 0;
		GLuint m_FontTexture;
		render::RenderDevice* m_pRenderer;
		render::Pipeline* m_pShader;
		render::PipelineParam* m_uMVP;
		render::VertexBuffer* m_VBO;
		render::VertexArray* m_VAO;
		render::IndexBuffer* m_IBO;
		glm::mat4 ortho;
		float m_Time;
		GLFWwindow* m_pGLFW;
		bool m_MousePress[3];
		GLFWcursor*      g_MouseCursors[ImGuiMouseCursor_COUNT];
		bool             g_MouseJustPressed[5] = { false, false, false, false, false };

		ITimer* m_pTimer;
		IContext* pContext;
	public:

		OpenGLSysUI(IContext* pContext);

		~OpenGLSysUI();
		virtual void Update(float dt);
		virtual void Render();
		virtual const char* VGetName();
		void PostInit();
	private:
		void OnRenderDrawLists(ImDrawData* data);
		void CreateFontsTexture();
		void NewFrame();
		void ImGui_ImplGlfw_UpdateMousePosAndButtons();
		void ImGui_ImplGlfw_UpdateMouseCursor();
	};
}
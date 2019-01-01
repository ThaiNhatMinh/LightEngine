#pragma once

#include "..\Interface\ISysUI.h"
#include "..\Interface\Renderer.h"
#include "..\Interface\IRenderSystem.h"
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
		GLFWcursor*      m_MouseCursors[ImGuiMouseCursor_COUNT];
		bool             m_MouseJustPressed[5] = { false, false, false, false, false };

		ITimer* m_pTimer;
		IContext* pContext;
		// Chain GLFW callbacks: our callbacks will call the user's previously installed callbacks, if any.
		GLFWmousebuttonfun   m_PrevUserCallbackMousebutton = NULL;
		GLFWscrollfun        m_PrevUserCallbackScroll = NULL;
		GLFWkeyfun           m_PrevUserCallbackKey = NULL;
		GLFWcharfun          m_PrevUserCallbackChar = NULL;
		resources::TextureData texData;
	public:

		OpenGLSysUI(IContext* pContext);

		~OpenGLSysUI();
		virtual void Update(float dt);
		virtual void Render();
		virtual const char* VGetName();
		void PostInit();
	private:
		void OnRenderDrawLists(ImDrawData* data);
		void CreateFontsTexture(render::IRenderSystem* pRS);
		void NewFrame();
		void ImGui_ImplGlfw_UpdateMousePosAndButtons();
		void ImGui_ImplGlfw_UpdateMouseCursor();
	};
}
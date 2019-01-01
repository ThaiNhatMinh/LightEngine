#pragma once
#include "..\Interface\ISubSystem.h"
namespace Light
{
	class SystemUI :public ISubSystem
	{
	private:
		Shader* m_pShader;
		std::unique_ptr<imguiMesh> m_Mesh;
		GLuint m_FontTexture;
		bool m_MousePress[3];
		float m_MouseWhell;
		GLFWwindow* w;

		float m_Time;
	protected:
		void CreateFontsTexture();

	public:
		SystemUI(Context* c);
		~SystemUI();
		void NewFrame();

		void Text(const char* fmt, ...);
		void Render();
		void OnRenderDrawLists(ImDrawData* data);
		friend void ImGui_ImplGlfwGL3_MouseButtonCallback(GLFWwindow* w, int button, int action, int /*mods*/);
		friend void ImGui_ImplGlfwGL3_ScrollCallback(GLFWwindow* w, double /*xoffset*/, double yoffset);
	};

}
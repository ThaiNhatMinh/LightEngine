#pragma once
#include <pch.h>
#include "../Graphics3D/Mesh.h"
#include "../Core/Context.h"
#include <imgui\imgui.h>

class Console: public ISubSystem
{

private:
	GLuint m_FontTexture;
	Shader* m_pShader;
	std::unique_ptr<imguiMesh> m_Mesh;
	GLFWwindow* w;
	bool m_MousePress[3];
	float m_MouseWhell;
	float m_Time;
	vector<string>			m_Items;
	bool					ScrollToBottom;
	char					InputBuf[256];
	bool					p_Open;
	bool					Show;
protected:
	void CreateFontsTexture();
	void NewFrame();
public:
	Console();
	~Console();
	virtual void Init(Context* c);
	virtual void ShutDown();
	void Draw();

	void OnRenderDrawLists(ImDrawData* data);

	void    ExecCommand(const char* command_line);
	void    AddLog(const char* fmt, ...);
	void	SetStatus(bool s) { Show = s; }
	bool	GetStatus() { return Show; }
	void	CheckStatus();
	friend void ImGui_ImplGlfwGL3_MouseButtonCallback(GLFWwindow* w, int button, int action, int /*mods*/);
	friend void ImGui_ImplGlfwGL3_ScrollCallback(GLFWwindow* w, double /*xoffset*/, double yoffset);

	int(*CallBack)(ImGuiTextEditCallbackData* testData) = [](ImGuiTextEditCallbackData* testData)
	{
		if (testData->EventChar == '`')
			return 1;
		return 0;
	};
};

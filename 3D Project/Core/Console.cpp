#include "pch.h"
#include "../Core/Context.h"
#include "Console.h"




Console::Console():Show(0)
{
	memset(InputBuf, 0, sizeof(InputBuf));
}

Console::~Console()
{
	
	
}

void Console::Init(Context* c)
{
	

	c->m_pConsole = std::unique_ptr<Console>(this);

	

}

void Console::ShutDown()
{
}

void Console::Draw()
{
	if (!Show) return;
	
	
	if (!ImGui::Begin("Console", nullptr, ImGuiWindowFlags_NoResize| ImGuiWindowFlags_NoCollapse))
	{
		ImGui::End();
		return;
	}

	ImGui::SetWindowSize(ImVec2(520, 400), ImGuiCond_Appearing);

	ImGui::BeginChild("ScrollingRegion", ImVec2(0, -ImGui::GetItemsLineHeightWithSpacing()), false, ImGuiWindowFlags_HorizontalScrollbar);
	if (ImGui::BeginPopupContextWindow())
	{
		ImGui::EndPopup();
	}

	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1)); // Tighten spacing
	
	for (size_t i = 0; i < m_Items.size(); i++)
	{
		string item = m_Items[i];
		
		ImVec4 col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f); // A better implementation may store a type per-item. For the sample let's just parse the text.
		if (strstr(item.c_str(), "[error]")) col = ImColor(1.0f, 0.4f, 0.4f, 1.0f);
		else if (strncmp(item.c_str(), "# ", 2) == 0) col = ImColor(1.0f, 0.78f, 0.58f, 1.0f);
		ImGui::PushStyleColor(ImGuiCol_Text, col);
		ImGui::TextUnformatted(item.c_str());
		ImGui::PopStyleColor();
	}

	if (ScrollToBottom)
		ImGui::SetScrollHere();
	ScrollToBottom = false;
	ImGui::PopStyleVar();
	ImGui::EndChild();
	ImGui::Separator();

	// Command-line

	if (ImGui::InputText("Input", InputBuf, IM_ARRAYSIZE(InputBuf), ImGuiInputTextFlags_EnterReturnsTrue| ImGuiInputTextFlags_CallbackCharFilter, CallBack, this))
	{
		//char* input_end = InputBuf + strlen(InputBuf);
		//while (input_end > InputBuf && input_end[-1] == ' ') { input_end--; } *input_end = 0;
		//if (InputBuf[0])
		//	ExecCommand(InputBuf);
		ExecCommand(InputBuf);
		//cout << InputBuf << endl;
		strcpy(InputBuf, "");
	}

	if (ImGui::IsItemHovered() || (ImGui::IsRootWindowOrAnyChildFocused() && !ImGui::IsAnyItemActive() && !ImGui::IsMouseClicked(0)))
		ImGui::SetKeyboardFocusHere(-1); // Auto focus previous widget

	ImGui::End();
	/*static float f = 0.0f;
	ImGui::Begin("Console", false, 0);
	ImGui::Text("Hello, world!");
	ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
	if (ImGui::Button("Test Window")) {};
	if (ImGui::Button("Another Window")) {}
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::End();
	int display_w, display_h;
	glfwGetFramebufferSize(this->w, &display_w, &display_h);
	glViewport(0, 0, display_w, display_h);
	//glClearColor(0.0f,1.0f,0.0f,1.0f);
	//glClear(GL_COLOR_BUFFER_BIT);*/
	
}



bool Console::RegisterVar(const char * command, void * address, int num, int size, ConVarType type)
{
	for (int i = 0; i < m_VarList.size(); i++)
	{
		if (m_VarList[i].command == command) return false;
	}

	ConsoleVar v;
	v.command = command;
	v.val = address;
	v.num = num;
	v.size = size;
	v.type = type;
	m_VarList.push_back(v);
	return 1;
}

void Console::ExecCommand(char * command_line)
{
	int len = strlen(command_line);
	while (command_line[len - 1] == ' ')
	{
		command_line[len - 1] = '\0';
		len--;
	}
	// get command
	char cmd[64];
	int i = 0;
	while (command_line[i] != ' '&&i<=len)
	{
		cmd[i] = command_line[i];
		i++;
	}
	cmd[i] = '\0';
	i++;

	// check command exist
	int j = 0;
	for (j = 0; j < m_VarList.size(); j++)
	{
		if (m_VarList[j].command == cmd) break;
	}

	if (j == m_VarList.size())
	{
		AddLog("[error] %s command not found.",cmd);
		return;
	}

	// if no param input, show current var
	if (i>= len)
	{
		char var[512];
		char number[512];
		int _i = 0;
		switch (m_VarList[j].type)
		{
		case TYPE_INT:
		{
			int* buffer = (int*)m_VarList[j].val;
			for (int k = 0; k < m_VarList[j].num; k++)
			{
				sprintf(number, "%d", buffer[k]);
				strcpy(&var[_i], number);
				_i += strlen(number) + 1;
			}
			var[_i + 1] = '\0';
			break;
		}
		case TYPE_FLOAT:
		{
			float* buffer = (float*)m_VarList[j].val;
			for (int k = 0; k < m_VarList[j].num; k++)
			{
				sprintf(number, "%f", buffer[k]);
				strcpy(&var[_i], number);
				_i += strlen(number) + 1;
			}
			var[_i + 1] = '\0';
			break;
		}

		AddLog(var);
		}
	}

	// get param from command
	char param[8][64];
	int k = 0;
	for (k = 0; k < m_VarList[j].num; k++)
		{
			int m = 0;
			while (command_line[i] != ' ' &&command_line[i]!='\0')
			{
				param[k][m] = command_line[i];
				i++;
				m++;
			}
			param[k][m] = '\0';
			i++;
		}
	
	// convert to type
	switch (m_VarList[j].type)
	{
	case TYPE_INT:
	{
		int* buffer = (int*)m_VarList[j].val;
		for (i = 0; i < k; i++)
			buffer[i] = atoi(param[i]);
		break;
	}
	case TYPE_FLOAT:
	{
		float* buffer = (float*)m_VarList[j].val;
		for (i = 0; i < k; i++)
			buffer[i] = atof(param[i]);
		break;
	}
	default:
		E_ERROR("Unknow type");
		break;
	}


	AddLog(command_line);

}

void Console::CheckStatus()
{
	ImGuiIO& io = ImGui::GetIO();
	
	static bool oldstatus = false;
	if (io.KeysDown[GLFW_KEY_GRAVE_ACCENT] && !oldstatus)
	{
		
		m_Context->m_pInput->LookAll();
		Show = !Show;
		if(Show) m_Context->m_pWindows->SetMouse(GLFW_CURSOR_NORMAL);
		else m_Context->m_pWindows->SetMouse(GLFW_CURSOR_DISABLED);
		strcpy(InputBuf, "");
	}
	oldstatus = io.KeysDown[GLFW_KEY_GRAVE_ACCENT];
}

void Console::AddLog(const char * fmt, ...) IM_FMTARGS(2)
{
	// FIXME-OPT
	char buf[1024];
	va_list args;
	va_start(args, fmt);
	vsnprintf(buf, IM_ARRAYSIZE(buf), fmt, args);
	buf[IM_ARRAYSIZE(buf) - 1] = 0;
	va_end(args);
	m_Items.push_back(string(buf));
	ScrollToBottom = true;
}

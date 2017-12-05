#include "pch.h"
#include "../Core/Context.h"
#include "Console.h"

void ImGui_ImplGlfwGL3_KeyCallback(GLFWwindow* w, int key, int, int action, int mods)
{
	ImGuiIO& io = ImGui::GetIO();
	if (action == GLFW_PRESS)
		io.KeysDown[key] = true;
	if (action == GLFW_RELEASE)
		io.KeysDown[key] = false;
	
	(void)mods; // Modifiers are not reliable across systems
	io.KeyCtrl = io.KeysDown[GLFW_KEY_LEFT_CONTROL] || io.KeysDown[GLFW_KEY_RIGHT_CONTROL];
	io.KeyShift = io.KeysDown[GLFW_KEY_LEFT_SHIFT] || io.KeysDown[GLFW_KEY_RIGHT_SHIFT];
	io.KeyAlt = io.KeysDown[GLFW_KEY_LEFT_ALT] || io.KeysDown[GLFW_KEY_RIGHT_ALT];
	io.KeySuper = io.KeysDown[GLFW_KEY_LEFT_SUPER] || io.KeysDown[GLFW_KEY_RIGHT_SUPER];
	
}

void ImGui_ImplGlfwGL3_CharCallback(GLFWwindow*, unsigned int c)
{
	
	ImGuiIO& io = ImGui::GetIO();
	if (c > 0 && c < 0x10000)
		io.AddInputCharacter((unsigned short)c);
}

void ImGui_ImplGlfwGL3_MouseButtonCallback(GLFWwindow* w, int button, int action, int /*mods*/)
{
	Context* pw = (Context*)glfwGetWindowUserPointer(w);
	pw->m_pConsole;
	if (action == GLFW_PRESS && button >= 0 && button < 3)
		pw->m_pConsole->m_MousePress[button] = true;
}

void ImGui_ImplGlfwGL3_ScrollCallback(GLFWwindow* w, double /*xoffset*/, double yoffset)
{
	Context* pw = (Context*)glfwGetWindowUserPointer(w);

	pw->m_pConsole->m_MouseWhell += (float)yoffset; // Use fractional mouse wheel.
}

void Console::CreateFontsTexture()
{
	// Build texture atlas
	ImGuiIO& io = ImGui::GetIO();
	unsigned char* pixels;
	int width, height;
	io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);   // Load as RGBA 32-bits (75% of the memory is wasted, but default font is so small) because it is more likely to be compatible with user's existing shaders. If your ImTextureId represent a higher-level concept than just a GL texture id, consider calling GetTexDataAsAlpha8() instead to save on GPU memory.

															  // Upload texture to graphics system
	
	glGenTextures(1, &m_FontTexture);
	glBindTexture(GL_TEXTURE_2D, m_FontTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

	// Store our identifier
	io.Fonts->TexID = (void *)(intptr_t)m_FontTexture;

}

void Console::NewFrame()
{
	ImGuiIO& io = ImGui::GetIO();

	// Setup display size (every frame to accommodate for window resizing)
	int w, h;
	int display_w, display_h;
	glfwGetWindowSize(this->w, &w, &h);
	glfwGetFramebufferSize(this->w, &display_w, &display_h);
	io.DisplaySize = ImVec2((float)w, (float)h);
	io.DisplayFramebufferScale = ImVec2(w > 0 ? ((float)display_w / w) : 0, h > 0 ? ((float)display_h / h) : 0);

	// Setup time step
	double current_time = glfwGetTime();
	io.DeltaTime = m_Time > 0.0 ? (float)(current_time - m_Time) : (float)(1.0f / 60.0f);
	m_Time = current_time;

	if (glfwGetWindowAttrib(this->w, GLFW_FOCUSED))
	{
		if (io.WantMoveMouse)
		{
			glfwSetCursorPos(this->w, (double)io.MousePos.x, (double)io.MousePos.y);   // Set mouse position if requested by io.WantMoveMouse flag (used when io.NavMovesTrue is enabled by user and using directional navigation)
		}
		else
		{
			double mouse_x, mouse_y;
			glfwGetCursorPos(this->w, &mouse_x, &mouse_y);
			io.MousePos = ImVec2((float)mouse_x, (float)mouse_y);   // Get mouse position in screen coordinates (set to -1,-1 if no mouse / on another screen, etc.)
		}
	}
	else
	{
		io.MousePos = ImVec2(-FLT_MAX, -FLT_MAX);
	}

	for (int i = 0; i < 3; i++)
	{
		// If a mouse press event came, always pass it as "mouse held this frame", so we don't miss click-release events that are shorter than 1 frame.
		io.MouseDown[i] = m_MousePress[i] || glfwGetMouseButton(this->w, i) != 0;
		m_MousePress[i] = false;
	}

	io.MouseWheel = m_MouseWhell;
	m_MouseWhell = 0.0f;

	

	// Start the frame. This call will update the io.WantCaptureMouse, io.WantCaptureKeyboard flag that you can use to dispatch inputs (or not) to your application.
	ImGui::NewFrame();


}



Console::Console():m_Time(0),Show(0)
{
	memset(InputBuf, 0, sizeof(InputBuf));
}

Console::~Console()
{
	m_Mesh->Shutdown();
	ImGui::Shutdown();
}

void Console::Init(Context* c)
{
	w = c->m_pWindows->Window();
	ImGuiIO& io = ImGui::GetIO();
	io.KeyMap[ImGuiKey_Tab] = GLFW_KEY_TAB;                         // Keyboard mapping. ImGui will use those indices to peek into the io.KeyDown[] array.
	io.KeyMap[ImGuiKey_LeftArrow] = GLFW_KEY_LEFT;
	io.KeyMap[ImGuiKey_RightArrow] = GLFW_KEY_RIGHT;
	io.KeyMap[ImGuiKey_UpArrow] = GLFW_KEY_UP;
	io.KeyMap[ImGuiKey_DownArrow] = GLFW_KEY_DOWN;
	io.KeyMap[ImGuiKey_PageUp] = GLFW_KEY_PAGE_UP;
	io.KeyMap[ImGuiKey_PageDown] = GLFW_KEY_PAGE_DOWN;
	io.KeyMap[ImGuiKey_Home] = GLFW_KEY_HOME;
	io.KeyMap[ImGuiKey_End] = GLFW_KEY_END;
	io.KeyMap[ImGuiKey_Delete] = GLFW_KEY_DELETE;
	io.KeyMap[ImGuiKey_Backspace] = GLFW_KEY_BACKSPACE;
	io.KeyMap[ImGuiKey_Enter] = GLFW_KEY_ENTER;
	io.KeyMap[ImGuiKey_Escape] = GLFW_KEY_ESCAPE;
	io.KeyMap[ImGuiKey_A] = GLFW_KEY_A;
	io.KeyMap[ImGuiKey_C] = GLFW_KEY_C;
	io.KeyMap[ImGuiKey_V] = GLFW_KEY_V;
	io.KeyMap[ImGuiKey_X] = GLFW_KEY_X;
	io.KeyMap[ImGuiKey_Y] = GLFW_KEY_Y;
	io.KeyMap[ImGuiKey_Z] = GLFW_KEY_Z;

	io.RenderDrawListsFn = [](ImDrawData* data)
	{
		static_cast<Console*>(ImGui::GetIO().UserData)->OnRenderDrawLists(data);
	};

	io.SetClipboardTextFn = [](void* user_data, const char* text)
	{ glfwSetClipboardString((GLFWwindow*)user_data, text); };
	io.GetClipboardTextFn = [](void* user_data) -> const char*
	{ return glfwGetClipboardString((GLFWwindow*)user_data); };
	io.ClipboardUserData = w;
	io.ImeWindowHandle = glfwGetWin32Window(w);

	io.UserData = this;

	io.Fonts->AddFontDefault();

	
	glfwSetMouseButtonCallback(w, ImGui_ImplGlfwGL3_MouseButtonCallback);
	glfwSetScrollCallback(w, ImGui_ImplGlfwGL3_ScrollCallback);
	glfwSetKeyCallback(w, ImGui_ImplGlfwGL3_KeyCallback);
	glfwSetCharCallback(w, ImGui_ImplGlfwGL3_CharCallback);

	c->m_pConsole = std::unique_ptr<Console>(this);

	m_pShader = c->m_pResources->GetShader("ImGuiShader");
	m_Mesh = std::make_unique<imguiMesh>();
	m_Mesh->Init();

	CreateFontsTexture();

}

void Console::ShutDown()
{
}

void Console::Draw()
{
	if (!Show) return;
	NewFrame();
	
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
	ImGui::Render();
}

void Console::OnRenderDrawLists(ImDrawData * draw_data)
{
	ImGuiIO& io = ImGui::GetIO();
	int fb_width = (int)(io.DisplaySize.x * io.DisplayFramebufferScale.x);
	int fb_height = (int)(io.DisplaySize.y * io.DisplayFramebufferScale.y);
	if (fb_width == 0 || fb_height == 0)
		return;
	draw_data->ScaleClipRects(io.DisplayFramebufferScale);

	// Setup render state: alpha-blending enabled, no face culling, no depth testing, scissor enabled, polygon fill
	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_SCISSOR_TEST);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// Setup viewport, orthographic projection matrix
	glViewport(0, 0, (GLsizei)fb_width, (GLsizei)fb_height);

	m_pShader->Use();
	glBindVertexArray(m_Mesh->VAO);
	glBindSampler(0, 0);
	mat4 ortho = glm::ortho(0.0f, io.DisplaySize.x, io.DisplaySize.y,0.0f);
	m_pShader->SetUniformMatrix("ProjMtx", glm::value_ptr(ortho));
	
	for (int n = 0; n < draw_data->CmdListsCount; n++)
	{
		const ImDrawList* cmd_list = draw_data->CmdLists[n];
		const ImDrawIdx* idx_buffer_offset = 0;

		glBindBuffer(GL_ARRAY_BUFFER, m_Mesh->VBO);
		glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)cmd_list->VtxBuffer.Size * sizeof(ImDrawVert), (const GLvoid*)cmd_list->VtxBuffer.Data, GL_STREAM_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Mesh->EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr)cmd_list->IdxBuffer.Size * sizeof(ImDrawIdx), (const GLvoid*)cmd_list->IdxBuffer.Data, GL_STREAM_DRAW);
		for (int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.Size; cmd_i++)
		{
			const ImDrawCmd* pcmd = &cmd_list->CmdBuffer[cmd_i];
			if (pcmd->UserCallback)
			{
				pcmd->UserCallback(cmd_list, pcmd);
			}
			else
			{
				glBindTexture(GL_TEXTURE_2D, (GLuint)(intptr_t)pcmd->TextureId);
				glScissor((int)pcmd->ClipRect.x, (int)(fb_height - pcmd->ClipRect.w), (int)(pcmd->ClipRect.z - pcmd->ClipRect.x), (int)(pcmd->ClipRect.w - pcmd->ClipRect.y));
				glDrawElements(GL_TRIANGLES, (GLsizei)pcmd->ElemCount, sizeof(ImDrawIdx) == 2 ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT, idx_buffer_offset);
			}
			idx_buffer_offset += pcmd->ElemCount;
		}
	}

	glDisable(GL_BLEND);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_SCISSOR_TEST);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Console::ExecCommand(const char * command_line)
{
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
		m_Context->m_pWindows->SetMouse(!Show);
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

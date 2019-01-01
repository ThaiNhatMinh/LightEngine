#include "pch.h"
#include "OpenGLSysUI.h"
#include "..\Core\OpenGLWindows.h"
#include "..\Interface\IResourceManager.h"


Light::OpenGLSysUI::OpenGLSysUI(IContext * pContext):m_Time(0), pContext(pContext)
{
	pContext->VAddSystem(this);
	m_pRenderer = pContext->GetSystem<render::IRenderSystem>()->GetRenderDevice();
	m_pTimer = pContext->GetSystem<ITimer>();
	
	m_pGLFW = static_cast<OpenGLWindows*>(pContext->GetSystem<Light::IWindow>())->GetGLFW();

	// Setup Dear ImGui binding
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;         // We can honor GetMouseCursor() values (optional)
	io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;          // We can honor io.WantSetMousePos requests (optional, rarely used)

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

	//io.RenderDrawListsFn = [](ImDrawData* data){static_cast<OpenGLSysUI*>(ImGui::GetIO().UserData)->OnRenderDrawLists(data);};

	io.SetClipboardTextFn = [](void* user_data, const char* text){ glfwSetClipboardString((GLFWwindow*)user_data, text); };
	io.GetClipboardTextFn = [](void* user_data) -> const char*{ return glfwGetClipboardString((GLFWwindow*)user_data); };
	io.ClipboardUserData = m_pGLFW;
	io.ImeWindowHandle = glfwGetWin32Window(m_pGLFW);

	io.UserData = this;

	io.Fonts->AddFontDefault();

	m_MouseCursors[ImGuiMouseCursor_Arrow] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
	m_MouseCursors[ImGuiMouseCursor_TextInput] = glfwCreateStandardCursor(GLFW_IBEAM_CURSOR);
	m_MouseCursors[ImGuiMouseCursor_ResizeAll] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);   // FIXME: GLFW doesn't have this.
	m_MouseCursors[ImGuiMouseCursor_ResizeNS] = glfwCreateStandardCursor(GLFW_VRESIZE_CURSOR);
	m_MouseCursors[ImGuiMouseCursor_ResizeEW] = glfwCreateStandardCursor(GLFW_HRESIZE_CURSOR);
	m_MouseCursors[ImGuiMouseCursor_ResizeNESW] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);  // FIXME: GLFW doesn't have this.
	m_MouseCursors[ImGuiMouseCursor_ResizeNWSE] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);  // FIXME: GLFW doesn't have this.
	m_MouseCursors[ImGuiMouseCursor_Hand] = glfwCreateStandardCursor(GLFW_HAND_CURSOR);

	// Setup display size (every frame to accommodate for window resizing)
	int w, h;
	int display_w, display_h;
	glfwGetWindowSize(m_pGLFW, &w, &h);
	glfwGetFramebufferSize(m_pGLFW, &display_w, &display_h);
	io.DisplaySize = ImVec2((float)w, (float)h);
	io.DisplayFramebufferScale = ImVec2(w > 0 ? ((float)display_w / w) : 0, h > 0 ? ((float)display_h / h) : 0);

	CreateFontsTexture(pContext->GetSystem<render::IRenderSystem>());

	

}

void Light::OpenGLSysUI::PostInit()
{
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	auto pResources = pContext->GetSystem<resources::IResourceManager>();
	auto pVS = m_pRenderer->CreateVertexShader(pResources->VGetShaderCode("imgui.vs")->Get());
	auto pFS = m_pRenderer->CreatePixelShader(pResources->VGetShaderCode("imgui.fs")->Get());
	m_pShader = m_pRenderer->CreatePipeline(pVS, pFS);
	m_uMVP = m_pShader->GetParam("uMVP");
	ortho = glm::ortho(0.0f, io.DisplaySize.x, io.DisplaySize.y, 0.0f);

	m_VBO = m_pRenderer->CreateVertexBuffer(0);
	m_IBO = m_pRenderer->CreateIndexBuffer(0);

	int stride = sizeof(ImDrawVert);
#define OFFSETOF(TYPE, ELEMENT) ((size_t)&(((TYPE *)0)->ELEMENT))

	render::VertexElement elements[] =
	{
		{render::SHADER_POSITION_ATTRIBUTE,	render::VertexElementType::VERTEXELEMENTTYPE_FLOAT,			2, stride,OFFSETOF(ImDrawVert, pos)},
		{render::SHADER_TEXCOORD_ATTRIBUTE,	render::VertexElementType::VERTEXELEMENTTYPE_FLOAT,			2, stride,OFFSETOF(ImDrawVert, uv)},
		{render::SHADER_COLOR_ATTRIBUTE,	render::VertexElementType::VERTEXELEMENTTYPE_UNSIGNED_BYTE,	4, stride,OFFSETOF(ImDrawVert, col)},
	};
	render::VertexDescription* pDesc = m_pRenderer->CreateVertexDescription(3, elements);
	m_VAO = m_pRenderer->CreateVertexArray(1, &m_VBO, &pDesc);
	delete pDesc;
}
Light::OpenGLSysUI::~OpenGLSysUI()
{
	ImGui::DestroyContext();
	delete m_pShader;
	delete m_VAO;
	delete m_VBO;
	delete m_IBO;

}

void Light::OpenGLSysUI::Update(float dt)
{
	NewFrame();
	values[values_offset] = dt;
	values_offset++;
	values_offset = values_offset % MaxData;
	ImGui::PlotLines("Frame Times", values, IM_ARRAYSIZE(values), values_offset);

	ImGui::Text("DelltaTime: %f",dt);
	ImGui::SameLine();
	ImGui::Text("FPS: %d", m_pTimer->VGetFPS());
}

void Light::OpenGLSysUI::Render()
{
	ImGui::Render();
	OnRenderDrawLists(ImGui::GetDrawData());
}

const char * Light::OpenGLSysUI::VGetName()
{
	static const char * p = typeid(ISysUI).name();
	return p;
}

void Light::OpenGLSysUI::OnRenderDrawLists(ImDrawData * draw_data)
{
	

	// Avoid rendering when minimized, scale coordinates for retina displays (screen coordinates != framebuffer coordinates)
	ImGuiIO& io = ImGui::GetIO();
	int fb_width = (int)(draw_data->DisplaySize.x * io.DisplayFramebufferScale.x);
	int fb_height = (int)(draw_data->DisplaySize.y * io.DisplayFramebufferScale.y);
	if (fb_width <= 0 || fb_height <= 0)
		return;
	draw_data->ScaleClipRects(io.DisplayFramebufferScale);

	// Setup viewport, orthographic projection matrix
	//glViewport(0, 0, (GLsizei)fb_width, (GLsizei)fb_height);
	m_pRenderer->SetBlend(true);
	m_pRenderer->SetPipeline(m_pShader);
	m_pRenderer->SetCullFace(false);
	m_pRenderer->SetDepth(false);
	m_pRenderer->SetVertexArray(m_VAO);
	
	m_uMVP->SetAsMat4(glm::value_ptr(ortho));

	for (int n = 0; n < draw_data->CmdListsCount; n++)
	{
		const ImDrawList* cmd_list = draw_data->CmdLists[n];
		const ImDrawIdx* idx_buffer_offset = 0;

		m_VBO->SetData(cmd_list->VtxBuffer.Size * sizeof(ImDrawVert), (const GLvoid*)cmd_list->VtxBuffer.Data, render::STREAM_DRAW);
		m_IBO->SetData(cmd_list->IdxBuffer.Size * sizeof(ImDrawIdx), (const GLvoid*)cmd_list->IdxBuffer.Data, render::STREAM_DRAW);
		
		m_pRenderer->SetIndexBuffer(m_IBO);
		for (int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.Size; cmd_i++)
		{
			const ImDrawCmd* pcmd = &cmd_list->CmdBuffer[cmd_i];
			if (pcmd->UserCallback)
			{
				pcmd->UserCallback(cmd_list, pcmd);
			}
			else
			{
				m_pRenderer->SetTexture(render::TextureUnit::UNIT_AMBIENT, (render::Texture*)pcmd->TextureId);
				m_pRenderer->SetScissor(true,(int)pcmd->ClipRect.x, (int)(fb_height - pcmd->ClipRect.w), (int)(pcmd->ClipRect.z - pcmd->ClipRect.x), (int)(pcmd->ClipRect.w - pcmd->ClipRect.y));
				m_pRenderer->DrawElement((GLsizei)pcmd->ElemCount, idx_buffer_offset);
			}
			idx_buffer_offset += pcmd->ElemCount;
		}
	}

	m_pRenderer->SetScissor(false);
}

void Light::OpenGLSysUI::CreateFontsTexture(render::IRenderSystem* pRS)
{
	// Build texture atlas
	ImGuiIO& io = ImGui::GetIO();
	unsigned char* pixels;
	int width, height,bpp;
	io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height,&bpp);   // Load as RGBA 32-bits (75% of the memory is wasted, but default font is so small) because it is more likely to be compatible with user's existing shaders. If your ImTextureId represent a higher-level concept than just a GL texture id, consider calling GetTexDataAsAlpha8() instead to save on GPU memory.

	
	texData.uiWidth = width;
	texData.uiHeight = height;
	texData.eTarget = render::TextureType::TEXTURE_2D;
	texData.eFormat = texData.iInternalFormat = render::ColorFormat::FORMAT_RGBA;
	unsigned char* pixels2 = new unsigned char[width*height*bpp];
	memcpy(pixels2, pixels, width*height*bpp);
	texData.pData = pixels2;
	
	// Store our identifier
	io.Fonts->TexID = (void *)pRS->VCreateTexture(&texData);
}

void Light::OpenGLSysUI::ImGui_ImplGlfw_UpdateMousePosAndButtons()
{
	// Update buttons
	ImGuiIO& io = ImGui::GetIO();
	for (int i = 0; i < IM_ARRAYSIZE(io.MouseDown); i++)
	{
		// If a mouse press event came, always pass it as "mouse held this frame", so we don't miss click-release events that are shorter than 1 frame.
		io.MouseDown[i] = m_MouseJustPressed[i] || glfwGetMouseButton(m_pGLFW, i) != 0;
		m_MouseJustPressed[i] = false;
	}

	// Update mouse position
	const ImVec2 mouse_pos_backup = io.MousePos;
	io.MousePos = ImVec2(-FLT_MAX, -FLT_MAX);
	const bool focused = glfwGetWindowAttrib(m_pGLFW, GLFW_FOCUSED) != 0;

	if (focused)
	{
		if (io.WantSetMousePos)
		{
			glfwSetCursorPos(m_pGLFW, (double)mouse_pos_backup.x, (double)mouse_pos_backup.y);
		}
		else
		{	
			double mouse_x, mouse_y;
			glfwGetCursorPos(m_pGLFW, &mouse_x, &mouse_y);
			io.MousePos = ImVec2((float)mouse_x, (float)mouse_y);
		}
	}
}

void Light::OpenGLSysUI::ImGui_ImplGlfw_UpdateMouseCursor()
{
	ImGuiIO& io = ImGui::GetIO();
	if ((io.ConfigFlags & ImGuiConfigFlags_NoMouseCursorChange) || glfwGetInputMode(m_pGLFW, GLFW_CURSOR) == GLFW_CURSOR_DISABLED)
		return;

	ImGuiMouseCursor imgui_cursor = ImGui::GetMouseCursor();
	if (imgui_cursor == ImGuiMouseCursor_None || io.MouseDrawCursor)
	{
		// Hide OS mouse cursor if imgui is drawing it or if it wants no cursor
		glfwSetInputMode(m_pGLFW, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	}
	else
	{
		// Show OS mouse cursor
		// FIXME-PLATFORM: Unfocused windows seems to fail changing the mouse cursor with GLFW 3.2, but 3.3 works here.
		glfwSetCursor(m_pGLFW, m_MouseCursors[imgui_cursor] ? m_MouseCursors[imgui_cursor] : m_MouseCursors[ImGuiMouseCursor_Arrow]);
		glfwSetInputMode(m_pGLFW, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
}
void Light::OpenGLSysUI::NewFrame()
{
	ImGuiIO& io = ImGui::GetIO();
	IM_ASSERT(io.Fonts->IsBuilt());     // Font atlas needs to be built, call renderer _NewFrame() function e.g. ImGui_ImplOpenGL3_NewFrame() 

	// Setup display size
	int w, h;
	int display_w, display_h;
	glfwGetWindowSize(m_pGLFW, &w, &h);
	glfwGetFramebufferSize(m_pGLFW, &display_w, &display_h);
	io.DisplaySize = ImVec2((float)w, (float)h);
	io.DisplayFramebufferScale = ImVec2(w > 0 ? ((float)display_w / w) : 0, h > 0 ? ((float)display_h / h) : 0);

	// Setup time step
	double current_time = glfwGetTime();
	io.DeltaTime = m_Time > 0.0 ? (float)(current_time - m_Time) : (float)(1.0f / 60.0f);
	m_Time = current_time;

	ImGui_ImplGlfw_UpdateMousePosAndButtons();
	ImGui_ImplGlfw_UpdateMouseCursor();

	ImGui::NewFrame();


}

#include "pch.h"

VGUI::VGUI(Context* pContext):ISubSystem(pContext),m_Root(new UIGroup(this))
{
	m_pWindows = pContext->GetSystem<Windows>();
	vec2 size = m_pWindows->GetWindowSize();
	m_Proj = glm::ortho(0.0f, size.x, size.y, 0.0f);

	m_UIShader = pContext->GetSystem<Resources>()->GetShader("UI");

	pContext->AddSystem(this);
}

VGUI::~VGUI()
{
}

char * VGUI::GetName()
{
	return "VGUI";
}

void VGUI::Render()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	m_UIShader->Use();
	m_UIShader->SetUniformMatrix("MVP", glm::value_ptr(m_Proj));
	m_Root->Render();
	glDisable(GL_BLEND);
}

void VGUI::Update(float dt)
{
	
	m_Root->Update(dt, m_pWindows->GetMousePos());
}

UIElement * VGUI::GetRoot()
{
	return m_Root.get();
}

bool VGUI::AddFont(const string & fontname, const string & fontfile)
{
	// check if exits
	auto result = m_FontLists.find(fontname);
	if (result != m_FontLists.end()) return false;

	m_FontLists.insert({ fontname,FTFont(fontfile) });

	return true;
}

Shader * VGUI::GetShader()
{
	return m_UIShader;
}

const mat4 & VGUI::GetProj()
{
	return m_Proj;
}

FTFont * VGUI::GetFont(const string & fontname)
{
	auto result = m_FontLists.find(fontname);
	if (result == m_FontLists.end()) return nullptr;

	return &result->second;
}

#include "pch.h"

VGUI::VGUI():m_Root(new UIElement())
{
}

VGUI::~VGUI()
{
}

void VGUI::Render()
{
	m_Root->Render();
}

void VGUI::Update(float dt)
{
	m_Root->Update(dt);
}

UIElement * VGUI::GetRoot()
{
	return m_Root.get();
}

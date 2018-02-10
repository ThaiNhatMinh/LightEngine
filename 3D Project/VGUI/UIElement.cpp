#include "pch.h"

void UIElement::Render()
{
	for (auto& el : m_Children) el->Render();
}

bool UIElement::Update(float dt)
{
	for (auto& el : m_Children) el->Update(dt);

	return true;
}

#include "pch.h"

uint32 UIElement::IDCount = 0;

UIElement::UIElement()
{
	m_ID = ++IDCount;
}

UIElement::~UIElement()
{
}

void UIElement::Render()
{
}

bool UIElement::Update(float dt, const vec2& mousePos)
{
	return true;
}

vec2 UIElement::GetPos()
{
	return m_Pos;
}

void UIElement::SetPos(const vec3 & pos)
{
	m_Pos = pos;
}

uint32 UIElement::GetID()
{
	return m_ID;
}

UIGroup::UIGroup()
{
}

void UIGroup::Render()
{
	for (auto& el : m_Childrens) el->Render();
}

bool UIGroup::Update(float dt, const vec2& mousePos)
{
	for (auto& el : m_Childrens) el->Update(dt, mousePos);

	return true;
}

void UIGroup::AddChild(UIElement * pChild)
{
	//cout << "VGUI add: " << typeid(*pChild).name() << endl;
	m_Childrens.push_back(std::unique_ptr<UIElement>(pChild));
}

void UIGroup::RemoveChild(uint32 id)
{
	for (auto itr = m_Childrens.begin(); itr != m_Childrens.end(); itr++)
	{
		if ((*itr)->GetID() == id)
		{
			m_Childrens.erase(itr);
			break;
		}
	}
}

void UIGroup::RemoveChild(UIElement * pChild)
{
	for (auto itr = m_Childrens.begin(); itr != m_Childrens.end(); itr++)
	{
		if ((*itr).get() == pChild)
		{
			m_Childrens.erase(itr);
			break;
		}
	}
}

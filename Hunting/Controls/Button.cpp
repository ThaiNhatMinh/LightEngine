#include "stdafx.h"
#include "Button.h"

Button::Button(render::Texture* pTex, std::string text, glm::vec2 pos, glm::vec2 size, std::function<void(Control*)> callback)
{
	color = glm::vec4(1);
	SetPos(pos);
	SetText(text);
	SetSize(size);
	this->Click = callback;
	this->pTex = pTex;
}

bool Button::OnInputEvent(Light::events::EvtInput * pEvent)
{
	if (pEvent->type == events::Type_MouseMove)
	{
		if (CheckInside(pEvent->_mouseMove.x, pEvent->_mouseMove.y))
		{
			isHovering = true;
		}
		else isHovering = false;
	}
	else if (pEvent->type == events::Type_MouseButton)
	{
		if (pEvent->_mouseButton.button == MouseButton::Left)
		{
			prevM = currentM;
			currentM = pEvent->_mouseButton;
			if (currentM.action == Release && prevM.action == Press)
				Click(this);
		}
	}
	return true;
}

void Button::Update(float dt)
{
}

void Button::Draw(Light::vgui::IVGUI * pVGUI)
{
	if (isHovering) color.a = 100.0/255;
	else color.a = 150/255.0;

	pVGUI->VDraw(pTex, pos,color);
	pVGUI->VDrawText(text, pos);
}

void Button::SetText(std::string text)
{
	this->text = text;
}

bool Button::CheckInside(int x, int y)
{
	if (x < pos.x) return false;
	if (y < pos.y) return false;
	if (x > pos.x + size.x) return false;
	if (y > pos.y + size.y) return false;

	return true;
}

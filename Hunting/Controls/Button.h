#pragma once
#include "Control.h"
#include <functional>
using namespace Light;
class Button : public Control
{
private:
	bool isHovering;
	std::string text;
	std::function<void(Control*)> Click;
	render::Texture* pTex;
public:
	Button(render::Texture* pTex, std::string text, glm::vec2 pos, glm::vec2 size,std::function<void(Control*)> callback);
	virtual bool OnInputEvent(Light::events::EvtInput* pEvent);
	virtual void Update(float dt);
	virtual void Draw(Light::vgui::IVGUI* pVGUI);
	
	void SetText(std::string text);
private:
	bool CheckInside(int x, int y);
};
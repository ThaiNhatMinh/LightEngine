#pragma once
#include <memory>
#include <glm/vec2.hpp>
#include <string>
#include <IVGUI.h>
#include <Events.h>
#include <IResourceManager.h>
using namespace Light;
class Control
{
public:
	typedef std::unique_ptr<Control> Ptr;
protected:
	glm::vec2 pos; 
	glm::vec2 size;
	glm::vec4 color;
	events::MouseAction currentM, prevM;
public:
	virtual bool OnInputEvent(Light::events::EvtInput* pEvent) = 0;
	virtual void Update(float dt) = 0;
	virtual void Draw(Light::vgui::IVGUI* pVGUI) = 0;

	void SetPos(glm::vec2 pos) {
		this->pos = pos;
	};
	void SetSize(glm::vec2 size) {
		this->size = size;
	};
	void SetColor(glm::vec4 color) {
		this->color = color;
	}
};
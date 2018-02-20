#pragma once

// Base UI Element
class VGUI;

class UIElement
{
public:
	UIElement();
	~UIElement();

	virtual void Render();
	virtual bool Update(float dt,const vec2& mousePos);

	virtual vec2 GetPos();
	virtual void SetPos(const vec3& pos);

	uint32 GetID();
protected:
	vec2	m_Pos;
	vec2	m_Rotate;
	uint32	m_ID;
private:
	static uint32 IDCount;
};

class UIGroup:public UIElement
{
public:
	UIGroup(VGUI* pVGUI);
	~UIGroup() = default;
	virtual void Render();
	virtual bool Update(float dt, const vec2& mousePos);

	void AddChild(UIElement* pChild);
	void RemoveChild(uint32 id);
	void RemoveChild(UIElement* pChild);
protected:
	std::vector<std::unique_ptr<UIElement>> m_Childrens;
};
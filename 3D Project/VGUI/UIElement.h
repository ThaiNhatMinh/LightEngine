#pragma once

// Base UI Control
class UIElement
{
public:
	UIElement() = default;
	~UIElement() = default;

	virtual void Render();
	virtual bool Update(float dt);
private:
	std::vector<std::unique_ptr<UIElement>> m_Children;
	vec2 m_Pos;
};
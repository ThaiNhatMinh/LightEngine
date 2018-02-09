#pragma once


class UIControl
{
public:
	UIControl() = default;
	~UIControl() = default;

	void Render();
	bool Update(float dt);
private:
	std::vector<std::unique_ptr<UIControl>> m_Children;
};
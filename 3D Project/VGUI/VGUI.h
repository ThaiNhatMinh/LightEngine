#pragma once


class VGUI
{
public:
	VGUI();
	~VGUI();

	void		Render();
	void		Update(float dt);
	UIControl*	GetRoot();
	bool		AddFont(const string& fontfile);

private:
	std::unique_ptr<UIControl> m_Root;
};
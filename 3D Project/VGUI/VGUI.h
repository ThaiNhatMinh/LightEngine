#pragma once

class VGUI : public ISubSystem
{
public:
	enum Control
	{
		CTRL_TEXT,
		CTRL_IMAGE,
		CTRL_COUNT,
	};
public:
	VGUI(Context* c);
	~VGUI();

	

	void		Render();
	void		Update(float dt);
	UIGroup*	GetRoot();
	bool		AddFont(const string& fontname,const string& fontfile);
	Shader*		GetShader();
	const mat4& GetProj();
	FTFont*		GetFont(const string& fontname);

	UIElement*	CreateElement(Control ctrl);

private:
	class UIFactoryInterface;
	template<class T>class UIFactory;
	std::vector<std::unique_ptr<UIFactoryInterface>>			m_ControlFactory;
	std::vector<std::unique_ptr<FTFont>>		m_FontLists;
	mat4						m_Proj;
	Shader*						m_UIShader;
	Windows*					m_pWindows;
	std::unique_ptr<UIGroup>	m_Root;
};



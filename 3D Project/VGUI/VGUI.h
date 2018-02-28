#pragma once


class VGUI : public ISubSystem
{
public:
	VGUI(Context* pContext);
	~VGUI();

	virtual char*	GetName()override;

	void			Render();
	void			Update(float dt);
	UIElement*		GetRoot();
	bool			AddFont(const string& fontname,const string& fontfile);
	Shader*			GetShader();
	const mat4&		GetProj();
	FTFont*			GetFont(const string& fontname);

private:
	mat4						m_Proj;
	Shader*						m_UIShader;
	std::map<string, FTFont>	m_FontLists;
	Windows*					m_pWindows;
	std::unique_ptr<UIGroup>	m_Root;
};
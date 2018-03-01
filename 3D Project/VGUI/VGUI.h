#pragma once


class VGUI : public ISubSystem
{
public:
	VGUI();
	~VGUI();

	virtual void Init(Context* c);
	//virtual void ShutDown() {};

	void		Render();
	void		Update(float dt);
	UIGroup*	GetRoot();
	bool		AddFont(const string& fontname,const string& fontfile);
	Shader*		GetShader();
	const mat4& GetProj();
	FTFont*		GetFont(const string& fontname);

private:
	mat4						m_Proj;
	Shader*						m_UIShader;
	std::vector<std::unique_ptr<FTFont>>	m_FontLists;
	Windows*					m_pWindows;
	std::unique_ptr<UIGroup>	m_Root;
};
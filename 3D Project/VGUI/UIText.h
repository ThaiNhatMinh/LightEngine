#pragma once

class FTFont;

class UIText : public UIElement
{
public:
	UIText(VGUI * pVGUI);
	~UIText()=default;

	virtual void Render()override;


	const string&	GetText()const;;
	void			SetText(const string& text);

private:
	string m_Text;
	Shader* m_UIShader;
	FTFont* m_Font;
	UIMesh m_Mesh;
};
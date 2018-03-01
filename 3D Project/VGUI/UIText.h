#pragma once

class FTFont;

class UIText : public UIElement
{
private:
	struct TextRenderInfo
	{
		UIMesh Mesh;
		GLuint texID;
	};
public:
	UIText(VGUI * pVGUI,const vec2& pos,const string& text);
	~UIText()=default;


	virtual void Render()override;


	const string&	GetText()const;;
	void			SetText(const string& text);

private:
	string m_Text;
	Shader* m_UIShader;
	FTFont* m_Font;
	std::vector<TextRenderInfo> m_Meshs; // each mesh for a character in text

};
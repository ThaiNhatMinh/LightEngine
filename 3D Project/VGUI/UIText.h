#pragma once

class FTFont;


class UIText : public UIElement
{
	
private:
	struct TextRenderInfo
	{
		TextRenderInfo()=default;
		UIMesh	Mesh;
		GLuint	texID;
		vec2	Pos;
		TextRenderInfo(TextRenderInfo&& other);
	};

public:
	UIText();
	~UIText()=default;
	virtual void OnInit(VGUI*);

	virtual void Render()override;


	const string&	GetText()const;;
	void			SetText(const string& text);
private:
	void UpdateInternalData();
private:
	string m_Text;
	Shader* m_UIShader;
	FTFont* m_Font;
	std::vector<TextRenderInfo> m_Meshs; // each mesh for a character in text

};
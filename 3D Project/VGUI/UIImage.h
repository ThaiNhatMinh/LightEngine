#pragma once

class UIImage :public UIElement
{
public:
	UIImage();
	~UIImage();

	virtual void OnInit(VGUI*);
	virtual void Render()override;

	Texture*	GetImage();
	void		SetImage(Texture* image);
protected:
	virtual void UpdateInternalData();
private:
	Shader *	m_UIShader;
	Texture*	m_Texture;
	UIMesh		m_Mesh;
};
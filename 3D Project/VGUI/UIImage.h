#pragma once

class UIImage :public UIElement
{
public:
	UIImage(Texture*);
	~UIImage();

	virtual void Render()override;

	Texture*	GetImage();;
	void		SetImage(Texture* image);;
private:
	vec2	m_Pos;
	Texture* m_Texture;
};
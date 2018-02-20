#include "pch.h"

UIImage::UIImage(Texture *p):m_Texture(p)
{
}

UIImage::~UIImage()
{
}

void UIImage::Render()
{
}

Texture * UIImage::GetImage() {
	return m_Texture;
}

void UIImage::SetImage(Texture * image) {
	m_Texture = image;
}

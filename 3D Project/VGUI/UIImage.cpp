#include "pch.h"

UIImage::UIImage(Texture *p):m_Texture(p)
{
}

UIImage::~UIImage()
{
}

Texture * UIImage::GetImage() {
	return m_Texture;
}

void UIImage::SetImage(Texture * image) {
	m_Texture = image;
}

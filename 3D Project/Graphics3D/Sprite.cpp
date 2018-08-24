#include <pch.h>

Sprite::Sprite(Texture * Tex) :m_Tex(Tex)
{
	size = vec2(Tex->GetWidth(), Tex->GetHeight());
}

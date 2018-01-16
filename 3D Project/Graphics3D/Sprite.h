#pragma once


class Texture;

class Sprite
{
	friend class EffectSystem;
public:
	Sprite(Texture *Tex);
	~Sprite() = default;

	
private:
	Texture		*m_Tex;
	vec3		m_Pos;
	vec2		m_Size;
};
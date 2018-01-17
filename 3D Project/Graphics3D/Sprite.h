#pragma once


class Texture;

class Sprite
{
	friend class EffectSystem;
public:
	Sprite(Texture *Tex);
	~Sprite() = default;

	vec3& GetPos() { return m_Pos; }
private:
	Texture		*m_Tex;
	vec3		m_Pos;
	vec2		m_Size;
};
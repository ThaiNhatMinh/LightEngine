#pragma once


class Texture;

class Sprite :public Particle
{
	friend class EffectSystem;
public:
	Sprite(Texture *Tex);
	~Sprite() = default;

	vec3& GetPos() { return Pos; }
private:
	Texture		*m_Tex;
};
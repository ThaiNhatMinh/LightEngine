#pragma once

class Texture;
class SpriteAnim: public Particle
{
	friend class Resources;

public:
	struct SpriteFrame
	{
		SpriteFrame(Texture* Tex) :Tex(Tex), Size(Tex->iWidth, Tex->iHeight) {}
		Texture*	Tex;
		vec2		Size;
	};

public:
							SpriteAnim();
							~SpriteAnim();

	const std::string&		GetFilePath();

	void					Update(float dt);
	
	const SpriteFrame&		GetCurrentFrame();
	const glm::vec3&		GetPos()const;
	glm::vec3&				GetPos();

private:
	
	std::vector<SpriteFrame>m_FrameLists;
	std::string				m_FilePath;
	uint32					m_MsFrameRate;
	uint32					m_MsAnimLength;
	bool					m_bKeyed;
	uint32					m_ColourKey;
	bool					m_bTranslucent;
	uint32					m_MsCurTime;
	uint32					m_Flags;
	uint32					m_iCurrentFrame;
};
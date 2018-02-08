#pragma once

class Texture;
class SpriteAnim: public Particle
{
	friend class Resources;

public:
	enum SpriteFlag
	{
		SF_LOOP = 1,
		SF_FOLLOWACTOR = 1 << 1
	};
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
							
							// return true if finish
	bool					Update(float dt);
	
	const SpriteFrame&		GetCurrentFrame();
	const glm::vec3&		GetPos()const;
	glm::vec3&				GetPos();

	uint32					GetFlag();
	void					SetFlag(uint32);

	void					ResetState();
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
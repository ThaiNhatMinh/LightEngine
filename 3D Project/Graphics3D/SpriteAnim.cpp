#include "pch.h"

SpriteAnim::SpriteAnim():m_Flags(0), m_MsCurTime(0), m_iCurrentFrame(0), m_MsFrameRate(0)
{
}

SpriteAnim::~SpriteAnim()
{
}

const std::string& SpriteAnim::GetFilePath()
{
	return m_FilePath;
}

bool SpriteAnim::Update(float dt)
{
	//if(m_Flags==)
	m_MsCurTime += dt * 1000;

	if (m_MsCurTime > m_MsAnimLength)
	{
		if (m_Flags&SF_LOOP)m_MsCurTime = 0;
		else return 0;
	}

	// Figure out current frame
	m_iCurrentFrame = (m_MsCurTime / (1000 / m_MsFrameRate)) % m_FrameLists.size();

	return 1;
	
}

const SpriteAnim::SpriteFrame & SpriteAnim::GetCurrentFrame()
{
	assert(m_iCurrentFrame >= 0 && m_iCurrentFrame < m_FrameLists.size());

	return m_FrameLists[m_iCurrentFrame];
}

const glm::vec3 & SpriteAnim::GetPos()const
{
	return Pos;
}
glm::vec3 & SpriteAnim::GetPos()
{
	return Pos;
}

uint32 SpriteAnim::GetFlag()
{
	return m_Flags;
}

void SpriteAnim::SetFlag(uint32 flag)
{
	m_Flags = flag;
}

void SpriteAnim::ResetState()
{
	m_MsCurTime = 0;
	m_iCurrentFrame = 0;
}

SpriteAnim::SpriteFrame::SpriteFrame(Texture * Tex) :Tex(Tex), Size(Tex->GetWidth(), Tex->GetHeight()) {}

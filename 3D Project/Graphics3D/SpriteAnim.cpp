#include "pch.h"

SpriteAnim::SpriteAnim()
{
}

SpriteAnim::~SpriteAnim()
{
}

const std::string& SpriteAnim::GetFilePath()
{
	return m_FilePath;
}

void SpriteAnim::Update(float dt)
{
	//if(m_Flags==)
	m_MsCurTime += dt * 1000;

	if (m_MsCurTime > m_MsAnimLength) m_MsCurTime = 0;

	// Figure out current frame
	m_iCurrentFrame = (m_MsCurTime / (1000 / m_MsFrameRate)) % m_FrameLists.size();
	
}

const SpriteAnim::SpriteFrame & SpriteAnim::GetCurrentFrame()
{
	assert(m_iCurrentFrame >= 0 && m_iCurrentFrame < m_FrameLists.size());

	return m_FrameLists[m_iCurrentFrame];
}

const glm::vec3 & SpriteAnim::GetPos()const
{
	return m_Pos;
}
glm::vec3 & SpriteAnim::GetPos()
{
	return m_Pos;
}
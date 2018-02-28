#include "pch.h"

const char* SoundSource3D::Name = "SoundSource3D";

bool SoundSource3D::VInit(const tinyxml2::XMLElement * pData)
{
	m_pSystem = m_Context->GetSystem<SoundEngine>()->GetFMODSystem();

	for (const tinyxml2::XMLElement *pNode = pData->FirstChildElement(); pNode; pNode = pNode->NextSiblingElement())
	{
		const char* pName = pNode->Name();
		if (!strcmp(pName, "Sound"))
		{
			const char* pTag = pNode->Attribute("Tag");
			FMOD::Sound* pSound = m_Context->GetSystem<Resources>()->GetSound(pTag);
			
			if (pSound) m_SoundMap.insert({ pTag,pSound });
		}
	}
	return true;
}

// for the editor

tinyxml2::XMLElement * SoundSource3D::VGenerateXml(tinyxml2::XMLDocument * p) {
	return nullptr;
}

const char * SoundSource3D::VGetName() const
{
	return Name;
}

bool SoundSource3D::Play(const string & name)
{
	auto result = m_SoundMap.find(name);
	if (result == m_SoundMap.end())
	{
		E_ERROR("Can't find sound name " + name);
		return false;
	}
	if (!result->second)
	{
		E_ERROR("Invaild Sound " + name);
		return 0;
	}
	FMOD::Channel* pChannel;
	m_pSystem->playSound(result->second, nullptr, 0, &pChannel);
	pChannel->set3DAttributes(&ToFMODVector(m_pOwner->VGetGlobalTransform()[3]),nullptr);
	return 1;
}

bool SoundSource3D::Play(const string & name, const vec3 & pos)
{
	auto result = m_SoundMap.find(name);
	if (result == m_SoundMap.end())
	{
		E_ERROR("Can't find sound name " + name);
		return false;
	}
	if (!result->second)
	{
		E_ERROR("Invaild Sound " + name);
		return 0;
	}
	FMOD::Channel* pChannel;
	m_pSystem->playSound(result->second, nullptr, 0, &pChannel);
	pChannel->set3DAttributes(&ToFMODVector(pos), nullptr);
	return 1;
}

bool SoundSource3D::AddSound(const string & tag)
{
	auto result = m_SoundMap.find(tag);
	if (result == m_SoundMap.end())
	{
		FMOD::Sound* pSound = m_Context->GetSystem<Resources>()->GetSound(tag);
		if (pSound) m_SoundMap.insert({ tag,pSound });
		return true;
	}

	return false;
}

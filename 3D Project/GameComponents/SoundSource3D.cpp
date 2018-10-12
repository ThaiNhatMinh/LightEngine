#include <pch.h>
#include "SoundSource3D.h"
#include "..\interface.h"
#include "..\Core\SoundEngine.h"
#include "..\ResourceManager\ResourceManager.h"

namespace Light
{

	FMOD_VECTOR ToFMODVector(vec3 v);

	bool SoundSource3D::VSerialize(IContext*pContext, const tinyxml2::XMLElement * pData)
	{
		m_pSystem = static_cast<SoundEngine*>(pContext->GetSystem<ISoundEngine>())->GetFMODSystem();
		m_pResources = pContext->GetSystem<resources::IResourceManager>();
		for (const tinyxml2::XMLElement *pNode = pData->FirstChildElement(); pNode; pNode = pNode->NextSiblingElement())
		{
			const char* pName = pNode->Name();
			if (!strcmp(pName, "Sound"))
			{
				const char* pTag = pNode->Attribute("Tag");
				resources::Sound* pSound = pContext->GetSystem<resources::IResourceManager>()->VGetSound(pTag);

				if (pSound) m_SoundMap.insert({ pTag,static_cast<resources::FModSound*>(pSound)->pFMODSound });
			}
		}
		return true;
	}

	// for the editor

	tinyxml2::XMLElement * SoundSource3D::VDeserialize(tinyxml2::XMLDocument * p)
	{
		return nullptr;
	}


	bool SoundSource3D::Play(const string & name)
	{
		auto result = m_SoundMap.find(name);
		if (result == m_SoundMap.end())
		{
			E_ERROR("Can't find sound name %s", name.c_str());
			return false;
		}
		if (!result->second)
		{
			E_ERROR("Invaild Sound %s", name.c_str());
			return 0;
		}
		FMOD::Channel* pChannel;
		m_pSystem->playSound(result->second, nullptr, 0, &pChannel);
		pChannel->set3DAttributes(&ToFMODVector(m_pOwner->VGetGlobalTransform()[3]), nullptr);
		return 1;
	}

	bool SoundSource3D::Play(const string & name, const vec3 & pos)
	{
		auto result = m_SoundMap.find(name);
		if (result == m_SoundMap.end())
		{
			E_ERROR("Can't find sound name %s", name.c_str());
			return false;
		}
		if (!result->second)
		{
			E_ERROR("Invaild Sound  %s", name.c_str());
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
			resources::Sound* pSound = m_pResources->VGetSound(tag);
			if (pSound) m_SoundMap.insert({ tag,static_cast<resources::FModSound*>(pSound)->pFMODSound });
			return true;
		}

		return false;
	}
}

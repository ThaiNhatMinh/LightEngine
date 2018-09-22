#pragma once
#include "..\Interface\IComponent.h"

#include <fmod.hpp>
#include "..\Interface\IResourceManager.h"
namespace Light
{
	class SoundSource3D : public ISoundSource3D
	{
	public:

		SoundSource3D() = default;
		~SoundSource3D() = default;

		virtual bool VSerialize(IContext*pContext, const tinyxml2::XMLElement * pData);
		virtual tinyxml2::XMLElement* VDeserialize(tinyxml2::XMLDocument*p);

		virtual bool Play(const string& name);
		virtual bool Play(const string& name, const vec3& pos);
		virtual bool AddSound(const string& name);
	private:
		std::map<string, FMOD::Sound*> m_SoundMap;
		FMOD::System* m_pSystem;
		resources::IResourceManager* m_pResources;
	};
}
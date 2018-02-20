#pragma once


class SoundSource3D : public ActorComponent
{
public:
	static const char*	Name;
	SoundSource3D() = default;
	~SoundSource3D() = default;

	virtual bool VInit(const tinyxml2::XMLElement* pData);
	// for the editor
	virtual tinyxml2::XMLElement* VGenerateXml(tinyxml2::XMLDocument*p);;
	virtual const char *VGetName() const;

	bool Play(const string& name);
	bool Play(const string& name,const vec3& pos);
	bool AddSound(const string& name);
private:
	std::map<string, FMOD::Sound*> m_SoundMap;
	FMOD::System* m_pSystem;
};
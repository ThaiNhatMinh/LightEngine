#pragma once


class SoundListener : public ActorComponent
{
public:
	struct ListenerAttribute
	{
		int id;
		FMOD_VECTOR pos;
		FMOD_VECTOR vel;
		FMOD_VECTOR forward;
		FMOD_VECTOR up;
	};

	static const char*	Name;
	SoundListener() = default;
	~SoundListener() = default;

	virtual bool VInit(const tinyxml2::XMLElement* pData);
	virtual void VUpdate(float dt);
	// for the editor
	virtual tinyxml2::XMLElement* VGenerateXml(tinyxml2::XMLDocument*p) {
		return nullptr;
	};
	virtual const char *VGetName() const ;
	
	ListenerAttribute GetListenAttribute();

};
#pragma once


class ZombieController : public ActorComponent
{
public:
	ZombieController()=default;
	~ZombieController() = default;
	static const char*	Name;

	virtual bool VInit(const tinyxml2::XMLElement* pData);
	virtual tinyxml2::XMLElement* VGenerateXml(tinyxml2::XMLDocument*p);
	virtual void VPostInit(void);
	virtual void VUpdate(float dt);
	virtual void VPostUpdate();
	virtual void VOnChanged(void);

	virtual const char *VGetName() const;
};
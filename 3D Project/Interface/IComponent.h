#pragma once

class IComponent
{
public:
	virtual ~IComponent() = default;

	virtual bool VInit(const tinyxml2::XMLElement* pData) = 0;
	virtual tinyxml2::XMLElement* VGenerateXml(tinyxml2::XMLDocument*p) = 0;
	virtual void VPostInit(void) = 0;
	virtual void VUpdate(float dt) = 0;
	virtual void VPostUpdate() = 0;
	
	virtual const char *VGetName() const = 0;
	virtual IActor* VGetOwner() = 0;
};
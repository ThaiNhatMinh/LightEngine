#pragma once

class TerrainRenderComponent : public MeshRenderComponent
{
public:
	static const char* Name;
	virtual bool VInit(const tinyxml2::XMLElement* pData);
	virtual const char *VGetName() const { return Name; };
	~TerrainRenderComponent();
private:
	
};
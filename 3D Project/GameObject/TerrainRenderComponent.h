#pragma once

class TerrainRenderComponent : public MeshRenderComponent
{
public:
	static const char* Name;
	virtual bool VInit(const tinyxml2::XMLElement* pData);
	~TerrainRenderComponent();
private:
	Mesh* ReadFile(const char* filename);
};
#pragma once

class TerrainRenderComponent : public MeshRenderComponent
{
private:
	float m_fScale;
public:
	static const char* Name;
	virtual bool VInit(const tinyxml2::XMLElement* pData);
	virtual const char *VGetName() const { return Name; };

	virtual void Render(Scene*);
	~TerrainRenderComponent();
private:
	void GenerateMeshData(HeightMap* hm);
};
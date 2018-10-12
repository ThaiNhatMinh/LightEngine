#pragma once

class TerrainRenderComponent : public MeshRenderComponent
{
private:
	struct SubGrid : public Mesh
	{
		AABB box;
		SubGrid(const std::vector<DefaultVertex>& vertex, const std::vector<unsigned int> indices);
	};

	float m_fScale;
	Material m_Material;
public:
	static const char* Name;
	virtual bool VInit(const tinyxml2::XMLElement* pData);
	virtual const char *VGetName() const { return Name; };

	virtual void Render(Scene*);
	~TerrainRenderComponent();
private:
	void GenerateMeshData(HeightMap * hm, Texture* pText);
};
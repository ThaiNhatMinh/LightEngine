#pragma once
#include "..\pch.h"

struct DtxHeader
{
	unsigned int iResType;
	int iVersion;
	unsigned short usWidth;
	unsigned short usHeight;
	unsigned short usMipmaps;
	unsigned short usSections;
	int iFlags;
	int iUserFlags;
	unsigned char ubExtra[12];
	char szCommandString[128];
};

class Resources : public Singleton<Resources>
{
private:
	vector<Texture*> m_Textures;
	vector<ModelCache*> m_ModelCaches;
	map<string, Shader*> m_ShaderList;
	// this list store primitive shape 
	vector<IMesh*> m_PrimList;
private:
	Texture* HasTexture(const char* filename);
	ModelCache* HasModel(const char* filename);
	void ReleaseModel(ModelCache* p);

public:
	Resources();
	~Resources();
	virtual void  onStartUp();
	virtual void  onShutDown();


	Texture* LoadTexture(const char* filename);
	Texture* LoadCubeTex(const vector<string>& filelist);
	Texture* LoadTexMemory(const char* filename, unsigned char* data, int w, int h);
	Texture* LoadDTX(const char* filename);
	unsigned char* LoadHeightMap(const char* filename, int& w, int& h);
	ModelCache* LoadModel(const char* filename);
	ModelCache* LoadModelXML(const char* XMLFile);

	template <class shaderType>
	Shader* LoadShader(string key, const char* vs, const char* fs,bool linkshader = true);
	Shader* GetShader(string key);

	IMesh* CreateShape(ShapeType type);


};
template <class shaderType>
Shader * Resources::LoadShader(string key, const char * vs, const char* fs, bool linkshader)
{
	map<string, Shader*>::iterator pos = m_ShaderList.find(key);
	if (pos != m_ShaderList.end()) return pos->second;

	Shader* p = new shaderType(vs, fs);
	m_ShaderList.insert({ key, p });
	if (linkshader) p->LinkShader();
	return p;
}
Resources* gResources();
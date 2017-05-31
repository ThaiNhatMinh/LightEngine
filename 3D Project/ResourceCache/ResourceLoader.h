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

	Shader* LoadShader(string key, const char* vs, const char* fs);
	Shader* GetShader(string key);


};


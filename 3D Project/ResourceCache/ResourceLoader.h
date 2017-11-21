#pragma once
#include <pch.h>
#include "GameObject\ColliderComponent.h"

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
#define MAX_FILE_NAME 128
struct HeightMap
{
	char filename[MAX_FILE_NAME];
	GLuint Width=-1;
	GLuint Height=-1;
	float stepsize=-1;
	float minH, maxH;
	GLubyte* Data=nullptr;
};
class Resources : public Singleton<Resources>
{
private:
	vector<Texture*> m_Textures;
	vector<ModelCache*> m_ModelCaches;
	vector<HeightMap> m_HeightMaps;
	map<string, Shader*> m_ShaderList;
	// this list store primitive shape 
	vector<IMesh*> m_PrimList;
private:
	Texture* HasTexture(const char* filename);
	ModelCache* HasModel(const char* filename);
	HeightMap HasHeighMap(const char* filename);
	void ReleaseModel(ModelCache* p);

public:
	Resources();
	~Resources();
	virtual void  onStartUp();
	virtual void  onShutDown();

	HeightMap LoadHeightMap(const char* filename);
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
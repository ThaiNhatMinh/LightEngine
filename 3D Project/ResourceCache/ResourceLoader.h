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

class HeightMap
{
public:
	char filename[MAX_FILE_NAME];
	GLuint Width;
	GLuint Height;
	float stepsize;
	float hscale;
	float minH, maxH;
	GLubyte* Data;
	std::vector<std::unique_ptr<IMesh>> m_Mesh;
	~HeightMap()
	{
		delete[] Data;
	}
};


class Resources : public ISubSystem
{
private:
	vector<std::unique_ptr<Texture>> m_Textures;
	vector<std::unique_ptr<ModelCache>> m_ModelCaches;
	vector<std::unique_ptr<HeightMap>> m_HeightMaps;
	map<string, std::unique_ptr<Shader>> m_ShaderList;
	map<string, std::function<std::unique_ptr<Shader>(const char *, const char*)>> m_ShaderFactory;
	// this list store primitive shape 
	vector<IMesh*>	m_PrimList;
	// Default texture when can't found tex
	Texture* m_pDefaultTex;
	// Path to resource
	std::string		m_Path;
private:
	Texture* HasTexture(const char* filename);
	ModelCache* HasModel(const char* filename);
	HeightMap* HasHeighMap(const char* filename);


	HeightMap* LoadHeightMap(const char* filename, int size, int w, int h,float hscale, int sub);
	Texture* LoadTexture(const char* filename);
	Texture* LoadCubeTex(const vector<string>& filelist);
	Texture* LoadTexMemory(const char* filename, unsigned char* data, int w, int h);
	Texture* LoadDTX(const char* filename);
	unsigned char* LoadHeightMap(const char* filename, int& w, int& h);
	ModelCache* LoadModel(const char* filename);
	ModelCache* LoadModelXML(const char* XMLFile);
	IMesh* CreateShape(ShapeType type);
	
	Shader* LoadShader(string key, const char* vs, const char* fs, bool linkshader = true);

	void LoadResources(string path);

public:
	Resources();
	~Resources();
	virtual void  Init(Context* c);
	virtual void  ShutDown();

	
	Shader*		GetShader(string key);
	Texture*	GetTexture(const char* filename);
	ModelCache*	GetModel(const char* filename);
	HeightMap*	GetHeightMap(const char* filename);
	const char* GetPath(Shader* p) { return nullptr; };
	const char* GetPath(Texture* p) { return nullptr; };
	const char* GetPath(ModelCache* p) { return nullptr; };
	const char* GetPath(HeightMap* p) { return nullptr; };
};

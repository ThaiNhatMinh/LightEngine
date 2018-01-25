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
	string filename;
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

class SpriteAnim;

class Resources : public ISubSystem
{
private:
	vector<std::unique_ptr<Texture>> m_Textures;
	vector<std::unique_ptr<ModelCache>> m_ModelCaches;
	vector<std::unique_ptr<HeightMap>> m_HeightMaps;
	vector<std::unique_ptr<SpriteAnim>> m_SpriteLists;
	map<string, std::unique_ptr<Shader>> m_ShaderList;
	
	// this list store primitive shape 
	vector<std::unique_ptr<IMesh>>	m_PrimList;
	// Default texture when can't found tex
	Texture* m_pDefaultTex;
	// Path to resource
	std::string		m_Path;
private:
	Texture*		HasTexture(const string& filename);
	ModelCache*		HasModel(const string& filename);
	HeightMap*		HasHeighMap(const string& filename);
	SpriteAnim*		HasSprite(const string& filename);

	SpriteAnim*		LoadSpriteAnimation(const string& filename);
	HeightMap*		LoadHeightMap(const string& filename, int size, int w, int h,float hscale, int sub);
	Texture*		LoadTexture(const string& filename);
	Texture*		LoadCubeTex(const vector<string>& filelist);
	Texture*		LoadTexMemory(const string& filename, unsigned char* data, int w, int h);
	Texture*		LoadDTX(const string& filename);
	unsigned char*	LoadHeightMap(const string& filename, int& w, int& h);
	ModelCache*		LoadModel(const string& filename);
	ModelCache*		LoadModelXML(const string& filename);
	
	Shader* LoadShader(string key,const char* type, const char* vs, const char* fs, bool linkshader = true);

	void LoadResources(string path);

public:
	Resources();
	~Resources();
	virtual void  Init(Context* c);
	virtual void  ShutDown();

	IMesh*		CreateShape(ShapeType type,float* size);
	SpriteAnim*	GetSpriteAnimation(const string& filename);
	Shader*		GetShader(string key);
	Texture*	GetTexture(const string& filename);
	ModelCache*	GetModel(const string& filename);
	HeightMap*	GetHeightMap(const string& filename);

	const char* GetPath(Shader* p) { return nullptr; };
	const char* GetPath(Texture* p) { return nullptr; };
	const char* GetPath(ModelCache* p) { return nullptr; };
	const char* GetPath(HeightMap* p) { return nullptr; };
};

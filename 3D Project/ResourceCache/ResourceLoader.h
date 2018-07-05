#pragma once
#include <pch.h>
#include "GameObject\ColliderComponent.h"
#include <fmod.hpp>

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

class IMesh;
class HeightMap: public IResource
{
public:
	HeightMap(const std::string& path) :IResource(path) {}
	GLuint Width;
	GLuint Height;
	float stepsize;
	float hscale;
	float minH, maxH;
	GLubyte* Data;
	GLuint numSub;
	//std::unique_ptr<IMesh> m_Mesh;
	std::vector<DefaultVertex> m_Vertexs;
	std::vector<unsigned int>  m_Indices;
	~HeightMap()
	{
		delete[] Data;
	}
};

class SpriteAnim;

class Resources : public ISubSystem, public IResourceManager
{
private:
	class SoundRAAI
	{
	public:
		string FilePath;
		SoundRAAI(FMOD::Sound *p) :m_pSound(p) {};
		~SoundRAAI() { m_pSound->release(); };
		FMOD::Sound* GetSound() { return m_pSound; }
	private:
		FMOD::Sound* m_pSound;
	};
	vector<std::unique_ptr<Texture>> m_Textures;
	vector<std::unique_ptr<ModelCache>> m_ModelCaches;
	vector<std::unique_ptr<HeightMap>> m_HeightMaps;
	vector<std::unique_ptr<SpriteAnim>> m_SpriteLists;
	vector<std::unique_ptr<ObjModel>> m_ObjLists;
	map<string, std::unique_ptr<Shader>> m_ShaderList;
	map<string, std::unique_ptr<SoundRAAI>> m_SoundList;
	
	vector<std::unique_ptr<IMesh>>	m_PrimList;
	// Default texture when can't found tex
	Texture* m_pDefaultTex;
	// Path to resource
	std::string		m_Path;

	FMOD::System* m_FMOD;
private:
	Texture*		HasTexture(const string& filename);
	ModelCache*		HasModel(const string& filename);
	HeightMap*		HasHeighMap(const string& filename);
	SpriteAnim*		HasSprite(const string& filename);
	SoundRAAI*		HasSound(const string& tag);

	SpriteAnim*		LoadSpriteAnimation(const string& filename);
	HeightMap*		LoadHeightMap(const string& filename, int size, int w, int h,float hscale, int sub);
	Texture*		LoadTexture(const string& filename);
	Texture*		LoadCubeTex(const vector<string>& filelist);
	Texture*		LoadTexMemory(const string& filename, unsigned char* data, int w, int h);
	Texture*		LoadDTX(const string& filename);
	unsigned char*	LoadHeightMap(const string& filename, int& w, int& h);
	ModelCache*		LoadModel(const string& filename);
	ModelCache*		LoadModelXML(const string& filename);
	SoundRAAI*		LoadSound(const string& filename, const string& tag, int mode);
	Shader*			LoadShader(string key,const char* type, const char* vs, const char* fs, bool linkshader = true);
	ObjModel*		LoadObjModel(const std::string filename);

	void			LoadResources(string path);
	IMesh*			CreateShape(ShapeType type, float* size);
public:
	Resources(Context* c);
	~Resources();


	
	virtual SpriteAnim*		VGetSpriteAnimation(const string& filename)override;
	virtual Shader*			VGetShader(string key)override;
	virtual Texture*		VGetTexture(const string& filename)override;
	virtual IModelResource*	VGetModel(const string& filename)override;
	virtual HeightMap*		VGetHeightMap(const string& filename)override;
	virtual FMOD::Sound*	VGetSound(const string& tag)override;

	
};

#pragma once

class IResourceManager
{
public:
	virtual ~IResourceManager() = default;

	virtual SpriteAnim *	VGetSpriteAnimation(const string& filename) = 0;
	virtual Shader*			VGetShader(string key) = 0;
	virtual Texture*		VGetTexture(const string& filename) = 0;
	virtual IModelResource*	VGetModel(const string& filename) = 0;
	virtual HeightMap*		VGetHeightMap(const string& filename) = 0;
	virtual FMOD::Sound*	VGetSound(const string& tag) = 0;
};

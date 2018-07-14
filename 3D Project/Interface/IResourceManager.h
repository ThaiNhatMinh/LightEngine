#pragma once
#include <string>
#include <memory>
#include "..\Graphics3D\Renderer.h"

namespace Light
{
	namespace resources
	{
		class IResource
		{
		public:
			IResource(const std::string& path) :m_ResourcePath(path) {};
			virtual ~IResource() = default;

			const std::string& GetPath() { return m_ResourcePath; };
		private:
			std::string m_ResourcePath;
		};

		template<class T> 
		class ResourceHandle : public IResource
		{
		public:
				ResourceHandle(const std::string path,T* obj) :m_Object(obj) ,IResource(path){};
				T * Get(){return m_Object.get();};

		private:
			std::unique_ptr<T> m_Object;
		};
		class IResourceManager : public ISubSystem
		{
		public:
			virtual ~IResourceManager() = default;

			//virtual SpriteAnim *	VGetSpriteAnimation(const string& filename) = 0;
			//virtual Shader*			VGetShader(string key) = 0;
			virtual render::Texture*		VGetTexture(const std::string& filename) = 0;
			//virtual IModelResource*	VGetModel(const string& filename) = 0;
			//virtual HeightMap*		VGetHeightMap(const string& filename) = 0;
			//virtual FMOD::Sound*	VGetSound(const string& tag) = 0;
		};
	}
}

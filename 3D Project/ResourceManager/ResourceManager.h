#pragma once

#include <functional>
#include <mutex>

#include <thread>
#include "..\Graphics3D\Renderer.h"
#include "..\Graphics3D\ModelRender.h"
#include "..\Interface\IResourceManager.h"
#include "..\Graphics3D\Vertex.h"
#include "..\Graphics3D\DefaultModel.h"
#include "LTRawData.h"
#include "Interface/IFactory.h"
namespace Light
{
	namespace resources
	{
		static const char* SYSTEM_RESOURCES_CONFIG = "Configs\\Resources.xml";

		

		class ResourceManager : public IResourceManager
		{
		private:
			/*class SoundRAAI
			{
			public:
				string FilePath;
				SoundRAAI(FMOD::Sound *p) :m_pSound(p) {};
				~SoundRAAI() { m_pSound->release(); };
				FMOD::Sound* GetSound() { return m_pSound; }
			private:
				FMOD::Sound* m_pSound;
			};*/

			render::RenderDevice *m_pRenderDevice;
			std::vector<ResourceHandle<render::Texture>> m_Textures;
			std::vector<ResourceHandle<render::VertexShader>> m_VertexShaders;
			std::vector<ResourceHandle<render::PixelShader>> m_PixelShaders;
			std::vector<ResourceHandle<render::Model>> m_ModelCaches;
			std::vector<ResourceHandle<HeightMap>> m_HeightMaps;
			std::vector<ResourceHandle<SpriteData>> m_Sprites;
			
			//map<string, std::unique_ptr<SoundRAAI>> m_SoundList;

			//vector<std::unique_ptr<IMesh>>	m_PrimList;
			// Default texture when can't found tex
			render::Texture* m_pDefaultTex=nullptr;
			// Path to resource

			//FMOD::System* m_FMOD;
			IContext* m_pContext;
			
		private:

			class OpenGLContext
			{
			private:

				void * pthread;
			public:
				OpenGLContext(IContext* pContext);
				void MakeContext();
				~OpenGLContext();
			};
			typedef std::function<bool(const std::string&, const std::string&)> CheckResourceFunc;
			
			template<class T>T* HasResource(std::vector<ResourceHandle<T>>& list, const std::string& filepath, CheckResourceFunc func = [](const std::string&a, const std::string& b) {return a == b; });
			
			DefaultModel *			LoadObjModel(const std::string filename);
			SpriteData*				LoadSpriteAnimation(const std::string& filename);
			HeightMap*				LoadHeightMap(const std::string& filename, int size, int w, int h, float hscale, int sub);
			render::Texture*		LoadTexture(const std::string& filename);
			render::Texture*		LoadCubeTex(const std::vector<std::string>& filelist);
			render::Texture*		LoadDTX(const std::string& filename);
			render::Model*			LoadModel(const std::string& filename);
			//SoundRAAI*		LoadSound(const std::string& filename, const std::string& tag, int mode);
			render::VertexShader*	LoadVertexShader(const std::string& filepath);
			render::PixelShader*	LoadPixelShader(const std::string& filepath);

			void				LoadResources(const std::string path);
			void				LoadSystemResources();

			LTRawData*			LoadLTBModel(const std::string& filename);

			/*
				Load xml file store config for model (.LTB) and texture (.DTX)
				Return render::Model* interface
			*/
			render::Model*		LoadModelXML(const std::string& filename);
		public:
			ResourceManager(IContext* c);
			~ResourceManager();
			virtual const char*			VGetName()override;


			//virtual SpriteAnim*		VGetSpriteAnimation(const std::string& filename)override;
			//virtual Shader*			VGetShader(string key)override;
			virtual render::Texture*		VGetTexture(const std::string& filename)override;
			virtual render::VertexShader*	VGetVertexShader(const std::string& filename)override;
			virtual render::PixelShader*	VGetPixelShader(const std::string& filename)override;
			virtual render::Model *			VGetModel(const std::string& filename)override;
			virtual HeightMap*				VGetHeightMap(const std::string& filename)override;
			//virtual FMOD::Sound*	VGetSound(const std::string& tag)override;

		};

		
		

		template<class T>
		inline T * ResourceManager::HasResource(std::vector<ResourceHandle<T>>& list, const std::string& filepath,CheckResourceFunc func)
		{
			for (std::size_t i = 0; i < list.size(); i++)
				if (func(list[i].GetPath(),filepath))
					return list[i].Get();

			return nullptr;
		}
	}
}
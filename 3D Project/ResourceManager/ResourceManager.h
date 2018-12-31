#pragma once

#include <functional>
#include <mutex>

#include <thread>
#include "..\Interface\Renderer.h"
#include "..\Graphics3D\ModelRender.h"
#include "..\Interface\IResourceManager.h"
#include "..\Graphics3D\Vertex.h"
#include "..\Graphics3D\DefaultModel.h"
#include "Interface/IFactory.h"
#include "..\Utilities\LTBStruct.h"
#include <fmod.hpp>
namespace Light
{
	namespace resources
	{
		static const char* SYSTEM_RESOURCES_CONFIG = "Configs\\Resources.xml";

		struct LTRawMesh
		{
			std::vector<SkeVertex>				Vertexs;
			std::vector<unsigned int>				Indices;
			std::string								Name;
		};

		class DefaultShaderResource : public ShaderCode
		{
		public:
			std::unique_ptr<char[]> m_Object;
			std::size_t size;
			const char* Get() { return m_Object.get(); }
		};

		class BuildInShaderResource : public ShaderCode
		{
			const char* pCode;
		public:
			BuildInShaderResource(const char* code) :pCode(code) {}
			const char* Get() { return pCode; };
		};
		struct LTRawData :public ModelData
		{
			std::vector<LTRawMesh>		Meshs;
			std::vector<Light::SkeNode>		SkeNodes;
			std::vector<Light::WeightBlend>	wb;
			std::vector<std::string>	ChildName;
			std::vector<Light::Animation>		Anims;
			std::vector<Light::LTBSocket>		Sockets;
		};

		struct DefaultMeshData
		{
			std::vector<DefaultVertex>				Vertexs;
			std::vector<unsigned int>				Indices;
			std::string								Name;
		};

		struct DefaultModelData : public ModelData
		{
			std::vector<DefaultMeshData> Meshs;
			std::vector<TextureData*> Texs;
			std::vector<render::MaterialData> Mats;
			math::AABB Box;
		};


		class FModSound : public Sound
		{
		public:
			FModSound(FMOD::Sound* p) :pFMODSound(p) {};
			FMOD::Sound* pFMODSound;
		};
		class ResourceManager : public IResourceManager
		{
		private:
			std::vector<ResourceHandle<TextureData>> m_Textures;
			std::vector<ResourceHandle<ShaderCode>> m_ShaderCodes;
			std::vector<ResourceHandle<HeightMapData>> m_HeightMaps;
			std::vector<ResourceHandle<SpriteData>> m_Sprites;
			std::vector<ResourceHandle<ModelData>> m_Models;
			std::vector<ResourceHandle<Sound>> m_SoundList;
			std::vector<ResourceHandle<FontData>> m_FontDatas;
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
			
			ModelData *				LoadObjModel(const std::string filename);
			SpriteData*				LoadSpriteAnimation(const std::string& filename);
			HeightMapData*			LoadHeightMap(const std::string& filename);
			TextureData*			LoadTexture(const std::string& filename);
			TextureData*			LoadCubeTex(const std::vector<std::string>& filelist);
			TextureData*			LoadDTX(const std::string& filename);
			ModelData*				LoadModel(const std::string& filename);
			Sound*					LoadSound(const std::string& filename, int mode);
			ShaderCode*				LoadShaderCode(const std::string& filepath);
			void					LoadResources(const std::string path);
			LTRawData*				LoadLTBModel(const std::string& filename);

			void							LoadSystemResources();
		public:
			ResourceManager(IContext* c);
			~ResourceManager();
			virtual const char*			VGetName()override;


			virtual TextureData*			VGetTexture(const std::vector<std::string>& filename, bool isCube = false, bool tryload = false)override;
			virtual ShaderCode*				VGetShaderCode(const std::string& filename, bool tryload = false)override;
			virtual ModelData *				VGetModel(const std::string& filename, bool tryload = false)override;
			virtual HeightMapData*			VGetHeightMap(const std::string& filename, bool tryload = false)override;
			virtual Sound*					VGetSound(const std::string& tag, bool tryload = false)override;
			virtual SpriteData*				VGetSprite(const std::string& filename, bool tryload = false)override;
			virtual FontData*				VGetFont(const std::string& tag, bool tryload = false)override;
			void							PostInit();

		};

		
		

		template<class T>
		inline T * ResourceManager::HasResource(std::vector<ResourceHandle<T>>& list, const std::string& filepath,CheckResourceFunc func)
		{
			for (std::size_t i = 0; i < list.size(); i++)
				if (func(list[i].Get()->Path,filepath))
					return list[i].Get();

			return nullptr;
		}
	}
}
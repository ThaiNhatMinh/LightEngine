#pragma once
#include <string>
#include <memory>
#include "..\Graphics3D\Renderer.h"
#include "..\Graphics3D\ModelRender.h"
#include "..\typedef.h"
#include "..\Graphics3D\Vertex.h"
#include "..\ResourceManager\LTRawData.h"

namespace Light
{
	namespace resources
	{

		class HeightMap
		{
		public:
			uint32 Width;
			uint32 Height;
			float stepsize;
			float hscale;
			float minH, maxH;
			std::unique_ptr<uint8[]> Data;
			uint32 numSub;
			//std::unique_ptr<IMesh> m_Mesh;
			std::vector<DefaultVertex> m_Vertexs;
			std::vector<unsigned int>  m_Indices;

		};

		struct SpriteData
		{
			std::vector<std::string>m_FrameLists;
			uint32					m_MsFrameRate;
			uint32					m_MsAnimLength;
			bool					m_bKeyed;
			uint32					m_ColourKey;
			bool					m_bTranslucent;
			uint32					m_MsCurTime;
			uint32					m_Flags;
		};

		
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
			virtual render::Texture*		VGetTexture(const std::string& filename) = 0;
			virtual render::VertexShader*	VGetVertexShader(const std::string& key) = 0;
			virtual render::PixelShader*	VGetPixelShader(const std::string& key) = 0;
			virtual render::Model *			VGetModel(const std::string& filename) = 0;
			virtual HeightMap*				VGetHeightMap(const std::string& filename) = 0;
			virtual render::Texture*		VGetCubeTex(const std::vector<std::string>& filelist) = 0;
			virtual LTRawData*				VGetRawModel(const std::string& filename)=0;
			//virtual LoadStatus*				VLoadResource(const std::string& resourcePath, bool async = false) = 0;
			//virtual FMOD::Sound*	VGetSound(const string& tag) = 0;
		};
	}
}

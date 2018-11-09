#pragma once
#include <string>
#include <memory>
#include "Renderer.h"
#include "..\Graphics3D\ModelRender.h"
#include "..\typedef.h"
#include "..\Graphics3D\Vertex.h"
#include "..\Graphics3D\Sprite.h"

namespace Light
{
	namespace resources
	{
		class IResource
		{
		public:
			std::string Path;
		};

		struct ShaderCode : public IResource
		{
			std::unique_ptr<char[]> m_Object;
			std::size_t size;
			char* Get() { return m_Object.get(); }
		};

		struct ModelData : public IResource
		{
			virtual ~ModelData() = default;
		};

		

		struct HeightMapData : public IResource
		{
			uint32 Width;
			uint32 Height;			
			std::unique_ptr<uint8[]> Data;
			float minH, maxH;
		};

		enum TextureFlag
		{
			Flag_CubeTex = 1,
			Flag_Compress = 1 << 2,
			Flag_Normal = 1 << 3,
		};
		struct TextureData : public IResource
		{
			char alignment = 4;
			render::TextureType eTarget;			//  GL_TEXTURE_2D, GL_PROXY_TEXTURE_2D, GL_TEXTURE_1D_ARRAY, GL_PROXY_TEXTURE_1D_ARRAY, GL_TEXTURE_RECTANGLE, GL_PROXY_TEXTURE_RECTANGLE
			int iLevel;						// Specifies the level-of-detail number.
			render::ColorFormat iInternalFormat;			// Specifies the number of color components in the texture.
			unsigned int uiWidth, uiHeight;	// Specifies the width/height of the texture image
			render::ColorFormat eFormat;			// Specifies the format of the pixel data.
			render::NumberFormat eType;				// Specifies the data type of the pixel data
			void* pData;					// Specifies a pointer to the image data in memory. 
			TextureFlag flag;
			TextureData() {};
			~TextureData()
			{
				if (flag&Flag_CubeTex)
				{
					unsigned char** p = (unsigned char**)pData;
					for (int i = 0; i < 6; i++) delete[] p[i];
					delete[] p;
				}
				else delete[] pData;
			}
		};

		struct SpriteData : public IResource
		{
			std::vector<std::string>m_FrameLists;
			uint32					m_MsFrameRate;
			uint32					m_MsAnimLength;
			bool					m_bKeyed;
			uint32					m_ColourKey;
			bool					m_bTranslucent;
			uint32					m_Flags;
		};

		class Sound : public IResource
		{
		public:
			virtual ~Sound() = default;
		};

		
		template<class T> 
		class ResourceHandle
		{
		public:
			ResourceHandle(const std::string path, IResource* obj) :m_Object(static_cast<T*>(obj)) { obj->Path = path; };
				T * Get(){return m_Object.get();};
				

		private:
			std::unique_ptr<T> m_Object;
		};

		class IResourceManager : public ISubSystem
		{
		public:
			virtual ~IResourceManager() = default;

			
			virtual TextureData*			VGetTexture(const std::vector<std::string>& filename,bool isCube = false, bool tryload=false) = 0;
			//virtual render::VertexShader*	VGetVertexShader(const std::string& key, bool tryload = false) = 0;
			//virtual render::PixelShader*	VGetPixelShader(const std::string& key, bool tryload = false) = 0;
			virtual ShaderCode*				VGetShaderCode(const std::string& filename, bool tryload = false) = 0;
			virtual ModelData *				VGetModel(const std::string& filename, bool tryload = false) = 0;
			virtual HeightMapData*			VGetHeightMap(const std::string& filename, bool tryload = false) = 0;
			//virtual render::Texture*		VGetCubeTex(const std::vector<std::string>& filelist, bool tryload = false) = 0;
			//virtual LTRawData*				VGetRawModel(const std::string& filename, bool tryload = false)=0;
			virtual SpriteData*				VGetSprite(const std::string& filename, bool tryload = false) = 0;
			virtual Sound*					VGetSound(const std::string& tag, bool tryload = false) = 0;
		};
	}
}

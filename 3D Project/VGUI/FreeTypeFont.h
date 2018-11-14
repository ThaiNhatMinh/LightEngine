#pragma once

// FreeType 2.9
#include <ft2build.h>
#include FT_FREETYPE_H
#include "..\Interface\Renderer.h"
#include "..\Interface\IVGUI.h"
#include "..\Interface\IRenderSystem.h"
namespace Light
{
	namespace vgui
	{
		class FTFont: public Font
		{
		public:
			struct FontChar {
				render::Texture* iTextureID;
				std::size_t size[2];
				std::size_t Bearing[2];
				std::size_t advance;
			};

			FTFont(render::IRenderSystem* pRenderS,const string& name, const string& fontfile,int size);
			~FTFont();
			void			SetFontSize(int size);
			FontChar*		GetChar(FT_ULong c);
			virtual std::string GetName();

			FTFont& operator=(FTFont&& other) = delete;
			FTFont(FTFont &&other) = delete;
			FTFont& operator=(const FTFont& other) = delete;
			FTFont(const FTFont &other) = delete;

			static void InitFreeTypeFont();
			static void ReleaseFreeTypeFont();
		private:
			static FT_Library  m_library;
			FT_Face		m_face;
			std::map<FT_ULong, FontChar> m_CharMaps;
			std::vector<std::unique_ptr<resources::TextureData>> m_BufferList;
			std::string m_Name;
		};
	}
}
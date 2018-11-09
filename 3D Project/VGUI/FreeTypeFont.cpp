#include <pch.h>
#include "FreeTypeFont.h"
#include "..\Interface\IResourceManager.h"
FT_Library  Light::vgui::FTFont::m_library;
namespace Light
{
	namespace vgui
	{
		FTFont::FTFont(render::IRenderSystem* pRenderS,const string& name, const string& fontfile) :m_face(0)
		{
			auto pRenderD = pRenderS->GetRenderDevice();
			m_Name = name;
			if (FT_New_Face(m_library, fontfile.c_str(), 0, &m_face))
			{
				E_ERROR("Can't load fontface: %s", fontfile.c_str());
				return;
			}

			SetFontSize(22);

			FT_ULong  charcode;
			FT_UInt   gindex;

			charcode = FT_Get_First_Char(m_face, &gindex);
			//FT_GlyphSlot  slot = m_face->glyph;
			while (gindex)
			{

				if (FT_Load_Char(m_face, charcode, FT_LOAD_RENDER))
				{
					E_ERROR("Can't load char.");
					continue;
				}
				
				resources::TextureData* texData = DEBUG_NEW resources::TextureData;
				texData->eTarget = render::TEXTURE_2D;
				texData->iLevel = 0;
				texData->iInternalFormat = render::FORMAT_RED;
				texData->uiWidth = m_face->glyph->bitmap.width;
				texData->uiHeight = m_face->glyph->bitmap.rows;
				texData->eType = render::UNSIGNED_BYTE;
				texData->eFormat = texData->iInternalFormat;
				texData->pData = DEBUG_NEW char[texData->uiWidth*texData->uiHeight];
				memcpy(texData->pData, m_face->glyph->bitmap.buffer, texData->uiWidth*texData->uiHeight);
				texData->flag = resources::Flag_Normal;
				texData->alignment = 1;
				m_BufferList.push_back(std::unique_ptr<resources::TextureData>(texData));
				//
				
				// Now store character for later use
				FontChar character;
				character.iTextureID = pRenderS->VCreateTexture(texData);
				character.size[0] = m_face->glyph->bitmap.width;
				character.size[1] = m_face->glyph->bitmap.rows;
				character.Bearing[0] = m_face->glyph->bitmap_left;
				character.Bearing[1] = m_face->glyph->bitmap_top;
				character.advance = m_face->glyph->advance.x;

				m_CharMaps.insert(std::pair<FT_ULong, FontChar>(charcode, character));

				charcode = FT_Get_Next_Char(m_face, charcode, &gindex);

			}
			FT_Done_Face(m_face);
		}

		FTFont::~FTFont()
		{


		}

		void FTFont::SetFontSize(int size)
		{
			if (FT_Set_Pixel_Sizes(m_face, 0, size))
			{
				E_ERROR("Can't change font size.");
			}
		}

		FTFont::FontChar * FTFont::GetChar(FT_ULong c)
		{
			auto result = m_CharMaps.find(c);
			if (result == m_CharMaps.end()) return nullptr;

			return &result->second;
		}

		std::string FTFont::GetName()
		{
			return m_Name;
		}

		void FTFont::InitFreeTypeFont()
		{
			FT_Error error = FT_Init_FreeType(&m_library);
			if (error)
			{
				E_ERROR("Can't init freetype.");
				return;
			}
		}

		void FTFont::ReleaseFreeTypeFont()
		{
			FT_Done_FreeType(m_library);
		}

	}
}

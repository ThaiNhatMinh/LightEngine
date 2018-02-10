#include "pch.h"
#include "FreeTypeFont.h"

FTFont::FTFont(const string& fontfile)
{
	FT_Error error = FT_Init_FreeType(&m_library);
	if (error)
	{
		E_ERROR("Can't init freetype.");
		return;
	}

	
	if (FT_New_Face(m_library, fontfile.c_str(), 0, &m_face))
	{
		E_ERROR("Can't load fontface.");
		return;
	}
	
	SetFontSize(18);

	FT_ULong  charcode;
	FT_UInt   gindex;

	charcode = FT_Get_First_Char(m_face, &gindex);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	while (gindex != 0)
	{

		if (FT_Load_Char(m_face, charcode, FT_LOAD_RENDER))
		{
			E_WARNING("Can't load char.");
			continue;
		}

		GLuint texID;
		glGenTextures(1, &texID);
		glBindTexture(GL_TEXTURE_2D, texID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, m_face->glyph->bitmap.width, m_face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, m_face->glyph->bitmap.buffer);
		// Set texture options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// Now store character for later use
		FontChar character;
		character.iTextureID = texID;
		character.size[0] = m_face->glyph->bitmap.width;
		character.size[1] = m_face->glyph->bitmap.rows;
		character.Bearing[0] = m_face->glyph->bitmap_left;
		character.Bearing[1] = m_face->glyph->bitmap_top;
		character.advance = m_face->glyph->advance.x;

		m_CharMaps.insert(std::pair<GLchar, FontChar>(charcode, character));
		//cout << face->glyph->bitmap_left << " " << face->glyph->bitmap_top << endl;

		charcode = FT_Get_Next_Char(m_face, charcode, &gindex);

	}

}

FTFont::~FTFont()
{
	FT_Done_Face(m_face);
	FT_Done_FreeType(m_library);
}

void FTFont::SetFontSize(int size)
{
	if (FT_Set_Pixel_Sizes(m_face, 0, size))
	{
		E_ERROR("Can't change font size.");
	}
}

#pragma once

// FreeType 2.9
#include <ft2build.h>
#include FT_FREETYPE_H


class FTFont
{
public:
	struct FontChar {
		GLuint iTextureID;
		GLuint size[2];
		GLuint Bearing[2];
		GLuint advance;
	};

	FTFont(const string& fontfile);
	~FTFont();

	void SetFontSize(int size);

	FontChar* GetChar(char c);
private:
	FT_Library  m_library;
	FT_Face		m_face;
	std::map<FT_ULong, FontChar> m_CharMaps;
};
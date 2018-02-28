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

	FTFont(const string& name,const string& fontfile);
	~FTFont();
	void			SetFontSize(int size);
	FontChar*		GetChar(char c);
	const string&	GetName();

	FTFont& operator=(FTFont&& other);
	FTFont(FTFont &&other);

	static void InitFreeTypeFont();
	static void ReleaseFreeTypeFont();
private:
	static FT_Library  m_library;
	FT_Face		m_face;
	std::map<FT_ULong, FontChar> m_CharMaps;
	std::string m_Name;
};
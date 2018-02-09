#pragma once

// FreeType 2.9
#include <ft2build.h>
#include FT_FREETYPE_H

class FTFont
{
public:

	FTFont(const string& fontfile);
	~FTFont();

private:
	FT_Library  m_library;
};
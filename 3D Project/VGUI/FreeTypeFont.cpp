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

	FT_Face face;
	if (FT_New_Face(m_library, fontfile.c_str(), 0, &face))
	{
		E_ERROR("Can't load fontface.");
		return;
	}
	
}

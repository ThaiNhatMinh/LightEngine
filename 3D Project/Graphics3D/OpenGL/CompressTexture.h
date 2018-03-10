#pragma once

#include "Texture.h"

class CompressTexture: public Texture
{
public:
	CompressTexture(GLint internalformat, GLint w, GLint h, GLint size, GLubyte* data);
};
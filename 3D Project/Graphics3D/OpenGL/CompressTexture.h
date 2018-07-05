#pragma once

#include "Texture.h"

class CompressTexture: public Texture
{
public:
	CompressTexture(const std::string& name, const TextureCreateInfo& info);
};
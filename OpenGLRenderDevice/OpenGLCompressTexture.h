
#pragma once
#include <Renderer.h>
#include <glew.h>
#include "BaseTexture.h"
namespace Light {
	namespace render
	{
		class OpenGLCompressTexture : public BaseTexture
		{
		public:
			OpenGLCompressTexture(UINT32 target, int level, int internalformat, int width, int height, int border,int imageSize,const void * data);
			~OpenGLCompressTexture();


		};
	}
}
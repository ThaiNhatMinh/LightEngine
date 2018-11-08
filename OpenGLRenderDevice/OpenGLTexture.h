#pragma once
#include <Renderer.h>
#include <glew.h>
#include "BaseTexture.h"
namespace Light {
	namespace render
	{
		class OpenGLTexture : public BaseTexture
		{
		public:
			OpenGLTexture(UINT32 target, int level, int internalformat, int width, int height, int border, int format, int type, const void * data);
			~OpenGLTexture();

			
		};
	}
}
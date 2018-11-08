#pragma once
#include <Renderer.h>
#include "BaseTexture.h"

namespace Light
{
	namespace render
	{
		class OpenGLCubeTexture : public BaseTexture
		{
		public:

			OpenGLCubeTexture(UINT32 target, int level, int internalformat, int width, int height, int border, int format, int type, const void * data);
			~OpenGLCubeTexture();
		};
	}
}
#pragma once
#include "..\Renderer.h"


namespace Light
{
	namespace render
	{
		class OpenGLCubeTexture : public Texture
		{
			OpenGLCubeTexture(const TextureCreateInfo& info);
			~OpenGLCubeTexture();
		public:
			GLuint m_iHandle = 0;
		};
	}
}
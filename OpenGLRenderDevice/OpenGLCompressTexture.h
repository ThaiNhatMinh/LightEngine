
#pragma once
#include <Renderer.h>
#include <glew.h>
namespace Light {
	namespace render
	{
		class OpenGLCompressTexture : public Texture
		{
		public:
			OpenGLCompressTexture(const TextureCreateInfo& info);
			~OpenGLCompressTexture();

		public:
			GLuint m_iHandle = 0;

		};
	}
}
#pragma once
#include <Renderer.h>
#include <glew.h>
namespace Light {
	namespace render
	{
		class OpenGLTexture : public Texture
		{
		public:
			OpenGLTexture(const TextureCreateInfo& info);
			~OpenGLTexture();

		public:
			GLuint m_iHandle = 0;
			TextureCreateInfo m_TexInfo;
		};
	}
}
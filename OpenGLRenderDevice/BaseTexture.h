#pragma once
#include <Renderer.h>
#include <glew.h>
namespace Light
{
	namespace render
	{
		class BaseTexture : public Texture
		{
		public:
			virtual int GetWidth() {
				return W;
			};
			virtual int GetHeight() {
				return H;
			};

			int W, H;
			GLenum target;
			unsigned int m_iHandle;
		};
	}
}
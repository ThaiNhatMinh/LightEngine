#pragma once

#include "..\Renderer.h"

namespace Light
{
	namespace render
	{
		class OpenGLFrameBuffer : public FrameBuffer
		{
		public:
			GLuint m_iHandle;

		public:
			OpenGLFrameBuffer();
			~OpenGLFrameBuffer();

			virtual void AttachTexture(Attachment attachment, Texture* pTex, int level)override;
			virtual void AttachRenderBuffer(Attachment attachment, RenderBuffer* pBuffer)override;
			virtual void Begin();
			virtual void End();
		private:
			void CheckStatus();
		};
	}
}
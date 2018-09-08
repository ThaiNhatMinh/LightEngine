#pragma once

#include "..\Renderer.h"

namespace Light
{
	namespace render
	{
		class OpenGLBlendingState : public BlendingState
		{
		public:
			bool Enable;
			GLenum sfactor, dfactor;
			GLenum func;
			
		public:
			OpenGLBlendingState(BlendConfig config);
		};
	}
}
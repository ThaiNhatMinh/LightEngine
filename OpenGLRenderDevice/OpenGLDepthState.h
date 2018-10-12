#pragma once
#include <Renderer.h>
namespace Light
{
	namespace render
	{
		class OpenGLDepthState : public DepthState
		{
		public:

			OpenGLDepthState(bool enable, bool mask, Compare depthFunc);

		public:
			bool	DepthEnable;
			// Enable/Disable writing to depth buffer
			bool	DepthMask;
			GLenum	Depthfunc;
		};
	}
}
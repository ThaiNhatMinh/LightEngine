#pragma once
#include "../Renderer.h"
namespace Light
{
	namespace render
	{
		class OpenGLDepthStencilState : public DepthStencilState
		{
		public:

			OpenGLDepthStencilState(const DepthStencilConfig& config);

		public:
			bool	DepthEnable;
			// Enable/Disable writing to depth buffer
			bool	DepthMask;
			GLenum	Depthfunc;

			bool			FrontFaceStencilEnabled;
			GLenum			FrontFaceStencilCompare;
			GLenum			FrontFaceStencilFail;
			GLenum			FrontFaceStencilPass;
			GLenum			FrontFaceDepthFail;
			int				FrontFaceRef = 0;
			unsigned int	FrontFaceReadMask = 0xFFFFFFFF;
			unsigned int	FrontFaceWriteMask = 0xFFFFFFFF;

			bool			BackFaceStencilEnabled = false;
			GLenum			BackFaceStencilCompare = COMPARE_ALWAYS;
			GLenum			BackFaceStencilFail = STENCIL_KEEP;
			GLenum			BackFaceStencilPass = STENCIL_KEEP;
			GLenum			BackFaceDepthFail = STENCIL_KEEP;
			int				BackFaceRef = 0;
			unsigned int	BackFaceReadMask = 0xFFFFFFFF;
			unsigned int	BackFaceWriteMask = 0xFFFFFFFF;
		};
	}
}
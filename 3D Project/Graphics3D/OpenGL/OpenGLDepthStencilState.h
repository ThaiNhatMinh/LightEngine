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
			bool	DepthEnable = true;
			// Enable/Disable writing to depth buffer
			bool	DepthMask = true;
			Compare Depthfunc = COMPARE_LESS;

			bool			FrontFaceStencilEnabled = false;
			Compare			FrontFaceStencilCompare = COMPARE_ALWAYS;
			StencilAction	FrontFaceStencilFail = STENCIL_KEEP;
			StencilAction	FrontFaceStencilPass = STENCIL_KEEP;
			StencilAction	FrontFaceDepthFail = STENCIL_KEEP;
			int				FrontFaceRef = 0;
			unsigned int	FrontFaceReadMask = 0xFFFFFFFF;
			unsigned int	FrontFaceWriteMask = 0xFFFFFFFF;

			bool			BackFaceStencilEnabled = false;
			Compare			BackFaceStencilCompare = COMPARE_ALWAYS;
			StencilAction	BackFaceStencilFail = STENCIL_KEEP;
			StencilAction	BackFaceStencilPass = STENCIL_KEEP;
			StencilAction	BackFaceDepthFail = STENCIL_KEEP;
			int				BackFaceRef = 0;
			unsigned int	BackFaceReadMask = 0xFFFFFFFF;
			unsigned int	BackFaceWriteMask = 0xFFFFFFFF;
		};
	}
}
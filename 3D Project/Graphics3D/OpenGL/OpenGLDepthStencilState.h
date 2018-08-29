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

			bool			FrontStencilEnabled;
			GLenum			FrontStencilCompare;
			GLenum			FrontStencilFail;
			GLenum			FrontStencilPass;
			GLenum			FrontDepthFail;
			int				FrontRef = 0;
			unsigned int	FrontCompareMask;
			unsigned int	FrontWriteMask;

			bool			BackStencilEnabled;
			GLenum			BackStencilCompare;
			GLenum			BackStencilFail;
			GLenum			BackStencilPass;
			GLenum			BackDepthFail;
			int				BackRef;
			unsigned int	BackCompareMask;
			unsigned int	BackWriteMask;
		};
	}
}
#pragma once
#include <Renderer.h>
namespace Light
{
	namespace render
	{
		class OpenGLStencilState : public StencilState
		{
		public:

			OpenGLStencilState(const StencilConfig& config);

		public:
			

			bool			FrontStencilEnabled;
			GLenum			FrontStencilCompare;
			GLenum			FrontStencilFail;
			GLenum			FrontStencilPass;
			GLenum			FrontDepthFail;
			int				FrontRef;
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
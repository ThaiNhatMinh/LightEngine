#include "pch.h"
#include "OpenGLDepthStencilState.h"

Light::render::OpenGLDepthStencilState::OpenGLDepthStencilState(const DepthStencilConfig & config)
{
	static GLenum DepthFunc[] = {GL_NEVER,GL_LESS,GL_EQUAL,GL_LEQUAL,GL_GREATER,GL_NOTEQUAL,GL_GEQUAL,GL_ALWAYS};
	static GLenum StencilOp[] = { GL_KEEP,GL_ZERO,GL_REPLACE,GL_INCR,GL_INCR_WRAP,GL_DECR,GL_DECR_WRAP,GL_INVERT };
	DepthEnable = config.DepthEnable;
	DepthMask = config.DepthMask? GL_TRUE: GL_FALSE;
	Depthfunc = DepthFunc[config.Depthfunc];

	FrontStencilEnabled = config.FrontStencilEnabled;
	FrontStencilCompare = DepthFunc[config.FrontStencilCompare];
	FrontStencilFail = StencilOp[config.FrontStencilFail];
	FrontStencilPass = StencilOp[config.FrontStencilPass];
	FrontDepthFail = StencilOp[config.FrontDepthFail];
	FrontRef = config.FrontRef;
	FrontCompareMask = config.FrontCompareMask;
	FrontWriteMask = config.FrontWriteMask;

	/*BackStencilEnabled = config.BackStencilEnabled;
	BackStencilCompare = DepthFunc[config.BackStencilCompare];
	BackStencilFail = StencilOp[config.BackStencilFail];
	BackStencilPass = StencilOp[config.BackStencilPass];
	BackDepthFail = StencilOp[config.BackDepthFail];
	BackRef = config.BackRef;
	BackCompareMask = config.BackCompareMask;
	BackWriteMask = config.BackWriteMask;*/
}

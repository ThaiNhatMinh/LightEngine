#include "pch.h"
#include "OpenGLDepthStencilState.h"

Light::render::OpenGLDepthStencilState::OpenGLDepthStencilState(const DepthStencilConfig & config)
{
	static GLenum DepthFunc[] = {GL_NEVER,GL_LESS,GL_EQUAL,GL_LEQUAL,GL_GREATER,GL_NOTEQUAL,GL_GEQUAL,GL_ALWAYS};
	static GLenum StencilOp[] = { GL_KEEP,GL_ZERO,GL_REPLACE,GL_INCR,GL_INCR_WRAP,GL_DECR,GL_DECR_WRAP,GL_INVERT };
	DepthEnable = config.DepthEnable;
	DepthMask = config.DepthMask? GL_TRUE: GL_FALSE;
	Depthfunc = DepthFunc[config.Depthfunc];

	FrontFaceStencilEnabled = config.FrontFaceStencilEnabled;
	FrontFaceStencilCompare = DepthFunc[config.FrontFaceStencilCompare];
	FrontFaceStencilFail = StencilOp[config.FrontFaceStencilFail];
	FrontFaceStencilPass = StencilOp[config.FrontFaceStencilPass];
	FrontFaceDepthFail = StencilOp[config.FrontFaceDepthFail];
	FrontFaceRef = config.FrontFaceRef;
	FrontFaceReadMask = config.FrontFaceReadMask;
	FrontFaceWriteMask = config.FrontFaceWriteMask;

	BackFaceStencilEnabled = config.BackFaceStencilEnabled;
	BackFaceStencilCompare = DepthFunc[config.BackFaceStencilCompare];
	BackFaceStencilFail = StencilOp[config.BackFaceStencilFail];
	BackFaceStencilPass = StencilOp[config.BackFaceStencilPass];
	BackFaceDepthFail = StencilOp[config.BackFaceDepthFail];
	BackFaceRef = config.BackFaceRef;
	BackFaceReadMask = config.BackFaceReadMask;
	BackFaceWriteMask = config.BackFaceWriteMask;
}

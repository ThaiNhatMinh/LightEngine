#include "pch.h"
#include "OpenGLDepthStencilState.h"

Light::render::OpenGLDepthStencilState::OpenGLDepthStencilState(const DepthStencilConfig & config)
{
	DepthEnable = config.DepthEnable;
	DepthMask = config.DepthMask;
	Depthfunc = config.Depthfunc;
	FrontFaceStencilEnabled = config.FrontFaceStencilEnabled;
	FrontFaceStencilCompare = config.FrontFaceStencilCompare;
	FrontFaceStencilFail = config.FrontFaceStencilFail;
	FrontFaceStencilPass = config.FrontFaceStencilPass;
	FrontFaceDepthFail = config.FrontFaceDepthFail;
	FrontFaceRef = config.FrontFaceRef;
	FrontFaceReadMask = config.FrontFaceReadMask;
	FrontFaceWriteMask = config.FrontFaceWriteMask;

	BackFaceStencilEnabled = config.BackFaceStencilEnabled;
	BackFaceStencilCompare = config.BackFaceStencilCompare;
	BackFaceStencilFail = config.BackFaceStencilFail;
	BackFaceStencilPass = config.BackFaceStencilPass;
	BackFaceDepthFail = config.BackFaceDepthFail;
	BackFaceRef = config.BackFaceRef;
	BackFaceReadMask = config.BackFaceReadMask;
	BackFaceWriteMask = config.BackFaceWriteMask;
}

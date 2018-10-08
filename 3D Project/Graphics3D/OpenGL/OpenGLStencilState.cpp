#include "pch.h"
#include "OpenGLStencilState.h"
#include "DataMap.h"

Light::render::OpenGLStencilState::OpenGLStencilState(const StencilConfig & config)
{
	
	
	

	FrontStencilEnabled = config.FrontEnabled;
	FrontStencilCompare = DepthFunc[config.FrontCompare];
	FrontStencilFail = StencilOp[config.FrontStencilFail];
	FrontStencilPass = StencilOp[config.FrontPass];
	FrontDepthFail = StencilOp[config.FrontDepthFail];
	FrontRef = config.FrontRef;
	FrontCompareMask = config.FrontCompareMask;
	FrontWriteMask = config.FrontWriteMask;

	BackStencilEnabled = config.BackEnabled;
	BackStencilCompare = DepthFunc[config.BackCompare];
	BackStencilFail = StencilOp[config.BackStencilFail];
	BackStencilPass = StencilOp[config.BackPass];
	BackDepthFail = StencilOp[config.BackDepthFail];
	BackRef = config.BackRef;
	BackCompareMask = config.BackCompareMask;
	BackWriteMask = config.BackWriteMask;
}

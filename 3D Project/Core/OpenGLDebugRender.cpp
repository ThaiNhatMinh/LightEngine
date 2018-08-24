#include "pch.h"
#include "OpenGLDebugRender.h"

namespace Light
{
	OpenGLDebugRender::OpenGLDebugRender(IContext * pContext)
	{
		pContext->VAddSystem(this);
	}


}


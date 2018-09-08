#pragma once
#include "..\Renderer.h"

namespace Light
{
	namespace render
	{
		class OpenGLRasterState : public RasterState
		{
		public:
			bool CullEnable;
			GLenum FrontFaceWinding;
			GLenum CullFace;
			GLenum FillMode;

		public:
			OpenGLRasterState(CullFaceConfig config);

		};
	}
}
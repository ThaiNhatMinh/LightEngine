#pragma once

#include "..\Renderer.h"
#include <gl\glew.h>

namespace Light
{
	namespace render
	{
		class OpenGLPipelineParam : public PipelineParam
		{
		public:
			OpenGLPipelineParam(GLuint location, const char* name);
			virtual void SetAsInt(int value);
			virtual void SetAsFloat(float value);
			virtual void SetAsMat4(float* value);
			virtual void SetAsVec3(float* value);
			virtual void SetAsVec2(float* value);
		private:
			GLuint m_iLocation;
			std::string m_Name;
		};
	}
}
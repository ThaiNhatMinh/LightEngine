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
			virtual void SetAsInt(const int value);
			virtual void SetAsFloat(const float value);
			virtual void SetAsMat4(const float* value);
			virtual void SetAsVec3(const float* value);
			virtual void SetAsVec2(const float* value);
		private:
			GLuint m_iLocation;
			std::string m_Name;
		};
	}
}
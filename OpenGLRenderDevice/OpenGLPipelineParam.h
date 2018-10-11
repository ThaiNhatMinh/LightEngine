#pragma once

#include <Renderer.h>
#include <glew.h>

namespace Light
{
	namespace render
	{
		class OpenGLPipelineParam : public PipelineParam
		{
		public:
			OpenGLPipelineParam(GLuint location, const char* name);
			virtual void SetAsInt(const int value)override;
			virtual void SetAsFloat(const float value)override;
			virtual void SetAsMat4(const float* value)override;
			virtual void SetAsVec3(const float* value)override;
			virtual void SetAsVec2(const float* value)override;

			virtual void SetAsFloatV(const float* value, int num)override;
			virtual void SetAsMat4V(const float* value, int num)override;
			virtual void SetAsVec3V(const float* value, int num)override;
			virtual void SetAsVec2V(const float* value, int num)override;
		private:
			GLuint m_iLocation;
			std::string m_Name;
		};
	}
}
#pragma once

#include "..\Renderer.h"
#include <gl\glew.h>

#include <map>
#include <string>
namespace Light {
	namespace render
	{
		class NullPipelineParam : public PipelineParam
		{
			virtual void SetAsInt(const int value) {};
			virtual void SetAsFloat(const float value) {};
			virtual void SetAsMat4(const float* value) {};
			virtual void SetAsVec3(const float* value) {};
			virtual void SetAsVec2(const float* value) {};
		};

		class OpenGLPipeline : public Pipeline
		{
		public:
			OpenGLPipeline(VertexShader* pVertexShader, PixelShader* pPixelShader);
			~OpenGLPipeline();
			virtual PipelineParam* GetParam(const char* name) override;

		protected:
			void GetUniformList();
		public:
			GLuint m_iProgram;
		private:
			NullPipelineParam m_NullParam;
			std::map<std::string, std::unique_ptr<PipelineParam>> m_Uniforms;
		};
	}
}
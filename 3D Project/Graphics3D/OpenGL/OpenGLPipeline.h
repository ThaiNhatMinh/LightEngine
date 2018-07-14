#pragma once

#include "..\Renderer.h"
#include <gl\glew.h>

#include <map>
#include <string>
namespace Light {
	namespace render
	{
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
			std::map<std::string, std::unique_ptr<PipelineParam>> m_Uniforms;
		};
	}
}
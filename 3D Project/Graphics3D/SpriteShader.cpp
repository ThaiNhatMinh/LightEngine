#include "pch.h"

void SpriteShader::LinkShader()
{

	glBindAttribLocation(m_iProgramID, SHADER_POSITION_ATTRIBUTE, "pos");
	Shader::LinkShader();
}

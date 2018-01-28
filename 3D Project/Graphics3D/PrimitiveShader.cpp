#include "pch.h"

void PrimShader::SetupRender(Scene * pScene, Actor * pActor)
{
	this->Use();
	ICamera* pCam = Camera::GetCurrentCamera();
	// ----- Transform Matricies ------
	
	mat4 globalTransform = pActor->VGetGlobalTransform();
	SetUniformMatrix("Model", glm::value_ptr(globalTransform));
	mat4 MVP = pCam->GetVPMatrix()*globalTransform;
	SetUniformMatrix("MVP", glm::value_ptr(MVP));

	// ----- Lighting ------
	const Light& dirLight = pScene->GetDirLight();
	SetUniform("gLight.La", dirLight.La);
	SetUniform("gLight.Ld", dirLight.Ld);
	SetUniform("gLight.Ls", dirLight.Ls);
	SetUniform("gLight.direction", dirLight.direction);
	SetUniform("EyePos", pCam->GetPosition());
	
}

void PrimShader::LinkShader()
{
	glBindAttribLocation(m_iProgramID, SHADER_POSITION_ATTRIBUTE, "position");
	glBindAttribLocation(m_iProgramID, SHADER_NORMAL_ATTRIBUTE, "normal");
	glBindAttribLocation(m_iProgramID, SHADER_TEXCOORD_ATTRIBUTE, "uv");

	Shader::LinkShader();
}

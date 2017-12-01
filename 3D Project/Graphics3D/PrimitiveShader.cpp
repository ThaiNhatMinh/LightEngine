#include "pch.h"

void PrimShader::SetupRender(Scene * pScene, Actor * pActor)
{
	this->Use();
	// ----- Transform Matricies ------
	
	mat4 globalTransform = pActor->VGetGlobalTransform();
	SetUniformMatrix("Model", glm::value_ptr(globalTransform));
	mat4 MVP = pScene->GetViewProj()*globalTransform;
	SetUniformMatrix("MVP", glm::value_ptr(MVP));

	// ----- Lighting ------
	const Light& dirLight = pScene->GetDirLight();
	SetUniform("gLight.La", dirLight.La);
	SetUniform("gLight.Ld", dirLight.Ld);
	SetUniform("gLight.Ls", dirLight.Ls);
	SetUniform("gLight.direction", dirLight.direction);
	CameraComponent* pCam = pScene->GetCamera();
	if(pCam) SetUniform("EyePos", pCam->GetPosition());
	else
	{
		SetUniform("EyePos", pScene->GetDefaultCamera()->GetPosition());
	}
}

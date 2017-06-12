#include "..\pch.h"

void PrimShader::SetupRender(Scene * pScene, Actor * pActor)
{
	this->Use();
	// ----- Transform Matricies ------
	mat4 transform = pActor->GetComponent<TransformComponent>("TransformComponent")->GetTransform();
	mat4 parentTransform = pActor->VGetParent()->GetComponent<TransformComponent>("TransformComponent")->GetTransform();
	mat4 globalTransform = parentTransform*transform;
	SetUniformMatrix("Model", globalTransform.ToFloatPtr());
	mat4 MVP = globalTransform* pScene->GetViewProj();
	SetUniformMatrix("MVP", MVP.ToFloatPtr());
}

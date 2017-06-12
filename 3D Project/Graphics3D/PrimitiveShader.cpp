#include "..\pch.h"

void PrimShader::SetupRender(Scene * pScene, Actor * pActor)
{
	this->Use();
	// ----- Transform Matricies ------
	mat4 transform = pActor->GetComponent<TransformComponent>("TransformComponent")->GetTransform();
	mat4 parentTransform = pActor->VGetParent()->GetComponent<TransformComponent>("TransformComponent")->GetTransform();
	mat4 globalTransform = transform*parentTransform;
	SetUniformMatrix("Model", glm::value_ptr(globalTransform));
	mat4 MVP = pScene->GetViewProj()*globalTransform;
	SetUniformMatrix("MVP", glm::value_ptr(MVP));
}

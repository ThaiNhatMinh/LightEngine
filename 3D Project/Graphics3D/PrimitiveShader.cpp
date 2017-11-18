#include "pch.h"

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

	// ----- Lighting ------
	const Light& dirLight = pScene->GetDirLight();
	SetUniform("gLight.La", dirLight.La);
	SetUniform("gLight.Ld", dirLight.Ld);
	SetUniform("gLight.Ls", dirLight.Ls);
	SetUniform("gLight.direction", dirLight.direction);

	// ----- Material ------

	MeshRenderComponent* mrc = pActor->GetComponent<MeshRenderComponent>("MeshRenderComponent");
	if (!mrc) return;
	const Material& mat = mrc->GetMaterial();
	SetUniform("gMaterial.Ka", mat.Ka);
	SetUniform("gMaterial.Kd", mat.Kd);
	SetUniform("gMaterial.Ks", mat.Ks);
	SetUniform("gMaterial.exp", mat.exp);

}

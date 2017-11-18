#include "pch.h"

void SkeShader::SetupRender(Scene * pScene, Actor * pActor)
{
	this->Use();
	// ----- Transform Matricies ------
	mat4 globalTransform = pActor->VGetGlobalTransform();
	SetUniformMatrix("Model", glm::value_ptr(globalTransform));
	mat4 MVP = pScene->GetViewProj()*globalTransform;
	SetUniformMatrix("MVP", glm::value_ptr(MVP));

	// ----- Animation Matrix ------
	AnimationComponent* ac = pActor->GetComponent<AnimationComponent>("AnimationComponent");
	const vector<mat4>& tf = ac->GetTransform();
	for (size_t i = 0; i < tf.size(); i++)
	{
		this->SetUniformMatrix(m_BaseTransformLocation+i, glm::value_ptr(tf[i]));
	}

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

void SkeShader::LinkShader()
{
	Shader::LinkShader();

	for (auto it = m_UniformLists.begin(); it != m_UniformLists.end(); it++)
	{
		if (!(*it).first.compare(0, 9, "Transform"))
		{
			m_BaseTransformLocation = (*it).second;
			break;
		}
	}

#if 0
	int i = 0;
	int a = 0;
	while (a!=-1)
	{
		
		char buffer[4];
		_itoa(i, buffer, 10);
		string s = buffer;
		a = glGetUniformLocation(m_iProgramID, ("Transform[" + s + "]").c_str());

		cout << ("Transform[" + s + "]") << " " << a << endl;
		i++;
	}
#endif
}

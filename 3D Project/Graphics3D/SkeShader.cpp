#include <pch.h>

void SkeShader::SetupRender(Scene * pScene, Actor * pActor)
{
	this->Use();

	mat4 globalTransform = pActor->VGetGlobalTransform();
	BaseAnimComponent* ac = pActor->GetComponent<AnimationComponent>("AnimationComponent");
	if (!ac) ac = pActor->GetComponent<PVAnimationComponent>("PVAnimationComponent");
	ICamera* pCam = pScene->GetCurrentCamera();
	
	// ----- Transform Matricies ------
	
	SetUniformMatrix("Model", glm::value_ptr(globalTransform));
	mat4 MVP = pCam->GetVPMatrix()*globalTransform;
	SetUniformMatrix("MVP", glm::value_ptr(MVP));

	// ----- Animation Matrix ------
	
	const vector<mat4>& tf = ac->GetVertexTransform();
	for (size_t i = 0; i < tf.size(); i++)
	{
		this->SetUniformMatrix(m_BaseTransformLocation+i, glm::value_ptr(tf[i]));
	}

	// ----- Lighting ------
	const DirectionLight& dirLight = pScene->GetDirLight();
	SetUniform("gLight.La", dirLight.La);
	SetUniform("gLight.Ld", dirLight.Ld);
	SetUniform("gLight.Ls", dirLight.Ls);
	SetUniform("gLight.direction", dirLight.direction);
}

void SkeShader::LinkShader()
{
	glBindAttribLocation(m_iProgramID, SHADER_POSITION_ATTRIBUTE, "position");
	glBindAttribLocation(m_iProgramID, SHADER_NORMAL_ATTRIBUTE, "normal");
	glBindAttribLocation(m_iProgramID, SHADER_TEXCOORD_ATTRIBUTE, "uv");
	glBindAttribLocation(m_iProgramID, SHADER_BLEND1_ATTRIBUTE, "ws1");
	glBindAttribLocation(m_iProgramID, SHADER_BLEND2_ATTRIBUTE, "ws2");
	glBindAttribLocation(m_iProgramID, SHADER_BLEND3_ATTRIBUTE, "ws3");
	glBindAttribLocation(m_iProgramID, SHADER_BLEND4_ATTRIBUTE, "ws4");

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

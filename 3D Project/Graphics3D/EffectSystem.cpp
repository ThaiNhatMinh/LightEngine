#include "pch.h"

void EffectSystem::Init(Context * c)
{
	const GLfloat g_vertex_buffer_data[] = { -0.5f,  0.5f, 0.0f, 
	 -0.5f, -0.5f, 0.0f, 
	 0.5f,  0.5f, 0.0f,
	 0.5f, -0.5f, 0.0f, };

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
	glEnableVertexAttribArray(SHADER_POSITION_ATTRIBUTE);
	glVertexAttribPointer(SHADER_POSITION_ATTRIBUTE,3,  GL_FLOAT,  GL_FALSE,   		0,  (void*)0    );

	m_pShader = c->m_pResources->GetShader("SpriteShader");
	c->m_pEffectSystem = std::unique_ptr<EffectSystem>(this);
}

void EffectSystem::ShutDown() {
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}

void EffectSystem::Render(Scene* pScene)
{
	
	m_pShader->Use();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	m_Context->m_pRenderer->SetDrawMode(GL_TRIANGLE_STRIP);
	m_Context->m_pRenderer->SetVertexArrayBuffer(VAO);
	CameraComponent* pCam = pScene->GetCamera();
	m_pShader->SetUniformMatrix("MVP", glm::value_ptr(pScene->GetViewProj()));
	mat4 ViewMatrix = pCam->GetViewMatrix();
	//m_pShader->SetUniform("CameraUp", ViewMatrix[0][1], ViewMatrix[1][1], ViewMatrix[2][1]);
	//m_pShader->SetUniform("CameraRight", ViewMatrix[0][0], ViewMatrix[1][0], ViewMatrix[2][0]);
	m_pShader->SetUniform("CameraUp",pCam->GetUp());
	m_pShader->SetUniform("CameraRight", -pCam->GetRight());

	for (auto& el : m_SpriteLists)
	{
		m_pShader->SetUniform("SpritePos", el.m_Pos);
		m_pShader->SetUniform("SpriteSize", el.m_Size);
		m_Context->m_pRenderer->SetTexture(el.m_Tex);
		m_Context->m_pRenderer->Draw(0, 4);
	}
	glDisable(GL_BLEND);
}

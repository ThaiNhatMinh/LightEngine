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

void EffectSystem::Update(Scene* pScene,float dt)
{
	ICamera* pCam = Camera::GetCurrentCamera();

	for (auto& el : m_List2)
	{
		el->Update(dt);
		el->CameraDistance = glm::length2(el->Pos - pCam->GetPosition());
	}
}

void EffectSystem::Render(Scene* pScene)
{
	ICamera* pCam = Camera::GetCurrentCamera();


	std::sort(m_List2.begin(), m_List2.end(), [](SpriteAnim*a, SpriteAnim*b) {return *a < *b; });

	m_pShader->Use();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	m_Context->m_pRenderer->SetDrawMode(GL_TRIANGLE_STRIP);
	m_Context->m_pRenderer->SetVertexArrayBuffer(VAO);
	
	m_pShader->SetUniformMatrix("MVP", glm::value_ptr(pCam->GetVPMatrix()));
	mat4 ViewMatrix = pCam->GetViewMatrix();
	m_pShader->SetUniform("CameraUp",pCam->GetUp());
	m_pShader->SetUniform("CameraRight", -pCam->GetRight());

	/*for (auto& el : m_SpriteLists)
	{
		m_pShader->SetUniform("SpritePos", el.Pos);
		m_pShader->SetUniform("SpriteSize", el.size);
		m_Context->m_pRenderer->SetTexture(el.m_Tex);
		m_Context->m_pRenderer->Draw(0, 4);
	}*/

	for (auto& el : m_List2)
	{
		m_pShader->SetUniform("SpritePos", el->GetPos());
		auto data = el->GetCurrentFrame();
		m_pShader->SetUniform("SpriteSize", data.Size);
		m_Context->m_pRenderer->SetTexture(data.Tex);
		m_Context->m_pRenderer->Draw(0, 4);
	}

	glDisable(GL_BLEND);
}

void EffectSystem::AddSprite(Sprite a) {
	m_SpriteLists.push_back(a);
}

void EffectSystem::AddSprite(SpriteAnim * a)
{
	m_List2.push_back(a);
}

#include "pch.h"

void EffectSystem::CreateSpriteEvent(std::shared_ptr<IEvent> pEvent)
{
	EvtRequestCreateSprite* p = static_cast<EvtRequestCreateSprite*>(pEvent.get());

	SpriteAnim* c = m_pResources->GetSpriteAnimation(p->GetFile());
	c->ResetState();
	c->GetPos() = p->GetPos();
	if (p->isLoop()) c->SetFlag(SpriteAnim::SF_LOOP);
	this->AddSprite(c);
}

EffectSystem::EffectSystem(Context * c):ISubSystem(c)
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
	glVertexAttribPointer(SHADER_POSITION_ATTRIBUTE, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	m_pResources = c->GetSystem<Resources>();
	m_pShader = m_pResources->GetShader("SpriteShader");
	c->AddSystem(this);

	m_pEventManager = c->GetSystem<EventManager>();
	m_pEventManager->VAddListener(MakeDelegate(this, &EffectSystem::CreateSpriteEvent), EvtRequestCreateSprite::sk_EventType);
	m_pRenderer =c->GetSystem<OpenGLRenderer>();
}

EffectSystem::~EffectSystem()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	m_pEventManager->VRemoveListener(MakeDelegate(this, &EffectSystem::CreateSpriteEvent), EvtRequestCreateSprite::sk_EventType);
}

char * EffectSystem::GetName()
{
	static char* name = "Effect";
	return name;
}


void EffectSystem::Update(Scene* pScene,float dt)
{
	ICamera* pCam = Camera::GetCurrentCamera();

	for (auto itr = m_List2.begin();itr!=m_List2.end(); itr++)
	{
		if (!(*itr)->Update(dt))
		{
			m_List2.erase(itr);
			//cout << "Remove: " << (*itr)->GetFilePath() << endl;
		}
		else (*itr)->CameraDistance = glm::length2((*itr)->Pos - pCam->GetPosition());
	}
}

void EffectSystem::Render(Scene* pScene)
{
	ICamera* pCam = Camera::GetCurrentCamera();


	//std::sort(m_List2.begin(), m_List2.end(), [](SpriteAnim*a, SpriteAnim*b) {return *a < *b; });

	m_pShader->Use();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	m_pRenderer->SetDrawMode(GL_TRIANGLE_STRIP);
	m_pRenderer->SetVertexArrayBuffer(VAO);
	
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
		data.Tex->Bind();
		m_pRenderer->Draw(0, 4);
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

void EffectSystem::AddTempSprite(SpriteAnim * a)
{

}

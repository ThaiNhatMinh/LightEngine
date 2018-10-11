#include <pch.h>
#include "EffectSystem.h"
#include "..\Core\Events.h"
#include "..\Interface\IEventManager.h"
#include "..\Graphics3D\ICamera.h"
namespace Light
{
	void EffectSystem::CreateSpriteEvent(std::shared_ptr<IEvent> pEvent)
	{
		events::EvtRequestCreateSprite* p = static_cast<events::EvtRequestCreateSprite*>(pEvent.get());

		auto r = m_pResources->VCreateSprite(p->File, p->Pos);
		r->m_Loop = p->Loop;
		m_SpriteLists.push_back(std::unique_ptr<render::Sprite>(r));

	}

	void EffectSystem::UpdateSprite(float dt, render::ICamera*pCam)
	{
		for (auto&el : m_SpriteLists)
		{
			auto* pData = el->m_pData;

			//if(m_Flags==)
			el->m_MsCurTime += dt * 1000;

			if (el->m_MsCurTime > pData->m_MsAnimLength&& el->m_Loop)
			{
				el->m_MsCurTime = 0;
				
			}

			// Figure out current frame
			el->m_iCurrentFrame = (el->m_MsCurTime / (1000 / pData->m_MsFrameRate)) % pData->m_FrameLists.size();
			
			el->CameraDistance = glm::length2(el->m_Pos - pCam->GetPosition());
		}
	}

	void EffectSystem::RenderSprite()
	{
	}

	const char * EffectSystem::VGetName()
	{
		static const char * p = typeid(IEffectSystem).name();
		return p;
	}

	EffectSystem::EffectSystem(IContext * c)
	{
		c->VAddSystem(this);
		m_pRenderer = c->GetSystem<render::RenderDevice>();
		m_pResources = c->GetSystem<resources::IResourceManager>();

		const GLfloat g_vertex_buffer_data[] = { -0.5f,  0.5f, 0.0f,
			-0.5f, -0.5f, 0.0f,
			0.5f,  0.5f, 0.0f,
			0.5f, -0.5f, 0.0f, };

		VBO = m_pRenderer->CreateVertexBuffer(sizeof(g_vertex_buffer_data), g_vertex_buffer_data);
		render::VertexElement elements = { render::SHADER_POSITION_ATTRIBUTE,render::VertexElementType::VERTEXELEMENTTYPE_FLOAT,3,0,0 };
		render::VertexDescription* pDesc = m_pRenderer->CreateVertexDescription(1, &elements);
		VAO = m_pRenderer->CreateVertexArray(1, &VBO, &pDesc);
		

		//VAO.SetAttibutePointer(SHADER_POSITION_ATTRIBUTE, 3, GL_FLOAT, 0, 0);
		
		m_pShader = m_pRenderer->CreatePipeline(m_pResources->VGetVertexShader("Sprite"), m_pResources->VGetPixelShader("Sprite"));
		m_uMVP = m_pShader->GetParam("uMVP");
		m_uCamRight = m_pShader->GetParam("uCameraRight");
		m_uCamUp = m_pShader->GetParam("uCameraUp");

		c->GetSystem<IEventManager>()->VAddListener(DEBUG_NEW EventDelegate<EffectSystem>(this, &EffectSystem::CreateSpriteEvent), events::EvtRequestCreateSprite::StaticType);
	}

	EffectSystem::~EffectSystem()
	{
		//glDeleteVertexArrays(1, &VAO);
		//glDeleteBuffers(1, &VBO);
		delete VAO;
		delete VBO;

		//m_Context->GetSystem<EventManager>()->VRemoveListener(MakeDelegate(this, &EffectSystem::CreateSpriteEvent), EvtRequestCreateSprite::sk_EventType);
	}

	void EffectSystem::VUpdate(float dt)
	{
		render::ICamera* pCam = m_pRenderer->VGetCurrentCamera();
		UpdateSprite(dt,pCam);
		//for (auto itr = m_List2.begin(); itr != m_List2.end(); itr++)
		//{
		//	if (!(*itr)->Update(dt))
		//	{
		//		m_List2.erase(itr);
		//		//cout << "Remove: " << (*itr)->GetFilePath() << endl;
		//	}
		//	else (*itr)->CameraDistance = glm::length2((*itr)->Pos - pCam->GetPosition());
		//}
	}

	void EffectSystem::VRender()
	{
		if (m_SpriteLists.size() == 0) return;
		render::ICamera* pCam = m_pRenderer->VGetCurrentCamera();


		//std::sort(m_SpriteLists.begin(), m_SpriteLists.end(), [](render::Sprite*a, render::Sprite*b) {return *a < *b; });

		m_pRenderer->SetPipeline(m_pShader);
		
		m_pRenderer->SetVertexArray(VAO);
		
		mat4 PV = pCam->GetProjMatrix() * pCam->GetViewMatrix();
		m_uMVP->SetAsMat4(glm::value_ptr(PV));
		mat4 ViewMatrix = pCam->GetViewMatrix();
		m_uCamUp->SetAsVec3(glm::value_ptr(pCam->GetUp()));
		m_uCamRight->SetAsVec3(glm::value_ptr(pCam->GetRight()));

		/*for (auto& el : m_SpriteLists)
		{
			m_pShader->SetUniform("SpritePos", el.Pos);
			m_pShader->SetUniform("SpriteSize", el.size);
			m_Context->m_pRenderer->SetTexture(el.m_Tex);
			m_Context->m_pRenderer->Draw(0, 4);
		}*/

		/*for (auto& el : m_List2)
		{
			m_pShader->SetUniform("SpritePos", el->GetPos());
			auto data = el->GetCurrentFrame();
			m_pShader->SetUniform("SpriteSize", data.Size);
			data.Tex->Bind();
			m_pRenderer->Draw(0, 4);
		}

		glDisable(GL_BLEND);*/
	}

	
}
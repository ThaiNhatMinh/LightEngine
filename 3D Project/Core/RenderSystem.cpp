#include "pch.h"
#include "RenderSystem.h"

#include "..\Graphics3D\RenderPass\DefaultPass.h"
#include "..\Graphics3D\SkyBox.h"
#include "..\Graphics3D\Scene.h"
#include "Events.h"
#include "EventManager.h"
#include "DebugRender.h"
#include "..\Graphics3D\EffectSystem.h"
namespace Light
{
	namespace render
	{
		RenderSystem::RenderSystem(IContext * pContext):m_pContext(pContext)
		{
			LoadRenderDevice();

			IWindow* pWindow = pContext->GetSystem<Light::IWindow>();

			int w, h;
			pWindow->VGetWindowSize(w, h);
			m_pRenderer->SetViewPort(0, 0, w, h);

			pContext->VAddSystem(this);
			pContext->GetSystem<IEventManager>()->VAddListener(DEBUG_NEW EventDelegate<RenderSystem>(this, &RenderSystem::OnObjectCreate), events::EvtNewActor::StaticType);
			pContext->GetSystem<IEventManager>()->VAddListener(DEBUG_NEW EventDelegate<RenderSystem>(this, &RenderSystem::OnCameraCreate), events::EvtCameraCreate::StaticType);
			pContext->GetSystem<IEventManager>()->VAddListener(DEBUG_NEW EventDelegate<RenderSystem>(this, &RenderSystem::OnbjectDestroy), events::EvtDestroyActor::StaticType);
			pContext->GetSystem<IEventManager>()->VAddListener(DEBUG_NEW EventDelegate<RenderSystem>(this, &RenderSystem::OnSceneCreate), events::EvtSceneCreate::StaticType);


			

			
		}

		void RenderSystem::Update(float dt)
		{
			m_pEffectSystem->VUpdate(dt);
		}


		void RenderSystem::Render()
		{
			m_pRenderer->Clear();
			// if there was no camera, so we can't see anything
			if (m_pCurrentCamera == nullptr) return;
			RenderData rd;
			rd.pCamera = m_pCurrentCamera;
			rd.pRenderer = m_pRenderer;
			rd.proj = m_pCurrentCamera->GetProjMatrix();
			rd.view = m_pCurrentCamera->GetViewMatrix();
			rd.pv = rd.proj * rd.view;
			

			{// pass default
				m_DefaultPass->Render(rd);
			}
			{// pass extra
				for (auto& pass : m_ExtraPass)
				{
					pass->Render(rd);
				}
			}
			
			m_pScene->VOnRender();
			m_pEffectSystem->VRender();
			m_pDebuger->Render();
		}

		render::ICamera * RenderSystem::VGetCurrentCamera()
		{
			return m_pCurrentCamera;
		}

		void RenderSystem::VSetCurrentCamera(render::ICamera * cam)
		{
			m_pCurrentCamera = cam;
		}

		void RenderSystem::AddExtraPass(RenderPass* pass)
		{
			auto r = std::find_if(m_ExtraPass.begin(), m_ExtraPass.end(), [pass](const std::unique_ptr<RenderPass>& renderPass)
			{
				return renderPass->GetName() == pass->GetName();
			});

			if (r != m_ExtraPass.end()) return;

			m_ExtraPass.push_back(std::unique_ptr<RenderPass>(pass));

		}

		RenderPass * RenderSystem::GetRenderPass(const std::string & name)
		{
			if (name == "Default") return m_DefaultPass.get();

			auto r = std::find_if(m_ExtraPass.begin(), m_ExtraPass.end(), [name](const std::unique_ptr<RenderPass>& renderPass)
			{
				return renderPass->GetName() == name;
			});

			if (r == m_ExtraPass.end()) return nullptr;

			return (*r).get();
		}

		Texture * RenderSystem::GetSkyBoxTexture()
		{
			SkyBox* pSky = static_cast<Scene*>(m_pScene)->GetSkyBox();
			return pSky->GetTexture();

		}

		IScene * RenderSystem::GetScene()
		{
			return m_pScene;
		}

		inline const char * RenderSystem::VGetName()
		{
			static const char* pName = typeid(IRenderSystem).name();
			return pName;
		}

		RenderDevice * RenderSystem::GetRenderDevice()
		{
			return m_pRenderer;
		}
		IDebugRender * RenderSystem::GetDebugRender()
		{
			return m_pDebuger.get();
		}
		IEffectSystem * RenderSystem::GetEffectSystem()
		{
			return m_pEffectSystem.get();
		}
		void RenderSystem::PostInit()
		{
			
			m_pEffectSystem = std::unique_ptr<IEffectSystem>(DEBUG_NEW EffectSystem(m_pContext));
			m_pDebuger = std::unique_ptr<IDebugRender>(DEBUG_NEW DebugRender(m_pContext));
			m_DefaultPass = std::unique_ptr<RenderPass>(DEBUG_NEW DefaultRenderPass("Default", m_pContext));
		}
		typedef Light::render::RenderDevice* (*CreateInterfaceFn)();
		void RenderSystem::LoadRenderDevice()
		{
			tinyxml2::XMLDocument doc;
			doc.LoadFile("Configs\\Plugin.xml");
			tinyxml2::XMLElement* pXMLNode = doc.FirstChildElement("RenderDevice");

			E_DEBUG(pXMLNode->GetText());
			HMODULE hModule = LoadLibrary(pXMLNode->GetText());
			if (!hModule)
			{
				E_ERROR("Can't load dll: %s", pXMLNode->Value());
			}

			

			CreateInterfaceFn fnCreateInterface = (CreateInterfaceFn)GetProcAddress((HMODULE)hModule, "CreateInterface");

			if (fnCreateInterface == nullptr)
			{
				E_ERROR("Can't load CreateInterface() ");
				
			}
			m_pRenderer = fnCreateInterface();

			

		
		}

		void RenderSystem::OnObjectCreate(std::shared_ptr<IEvent> event)
		{

			IActor* pActor = static_cast<events::EvtNewActor*>(event.get())->GetActor();

			AddObjectToPass(pActor, m_DefaultPass.get());

			for (auto& pass : m_ExtraPass)
				AddObjectToPass(pActor, pass.get());

		}

		void RenderSystem::OnbjectDestroy(std::shared_ptr<IEvent> event)
		{
			ActorId Actor = static_cast<events::EvtDestroyActor*>(event.get())->GetActor();

			m_DefaultPass->RemoveRenderObject(Actor);

			E_DEBUG("ObjectRender destroy: %d", Actor);

		}

		void RenderSystem::OnCameraCreate(std::shared_ptr<IEvent> event)
		{
			events::EvtCameraCreate* pEvent = static_cast<events::EvtCameraCreate*>(event.get());

			m_pCurrentCamera = pEvent->GetCamera();
			// just for debug, should remove in release

			E_DEBUG("Camera create!");

		}

		void RenderSystem::OnSceneCreate(std::shared_ptr<IEvent> event)
		{
			events::EvtSceneCreate* pEvent = static_cast<events::EvtSceneCreate*>(event.get());
			m_pScene = pEvent->m_pScene;
			E_DEBUG("SceneRender add: %s", m_pScene->VGetSceneName().c_str());
		}

		void RenderSystem::AddObjectToPass(IActor * pActor, RenderPass * pass)
		{

			IMeshRenderComponent* pMeshRender = pActor->GetComponent<IMeshRenderComponent>();
			ITransformComponent* pTransform = pActor->GetComponent<ITransformComponent>();
			IAnimatorComponent* pAnimatoor = pActor->GetComponent<IAnimatorComponent>();

			if (pMeshRender == nullptr) return;

			Renderable renderable;
			renderable.m_ActorID = pActor->VGetId();
			renderable.m_pActor = pActor;
			renderable.m_RenderComponent = pMeshRender;
			renderable.m_TransformComponent = pTransform;
			renderable.pAnimator = pAnimatoor;

			pass->AddRenderObject(renderable);

			E_DEBUG("ObjectRender add: %s", pActor->VGetName().c_str());
		}
	}
}
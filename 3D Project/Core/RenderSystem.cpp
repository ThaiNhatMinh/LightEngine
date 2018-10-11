#include "pch.h"
#include "RenderSystem.h"

#include "..\Graphics3D\RenderPass\DefaultPass.h"
#include "..\Graphics3D\SkyBox.h"
#include "..\Graphics3D\Scene.h"
#include "Events.h"
#include "EventManager.h"
namespace Light
{
	namespace render
	{
		RenderSystem::RenderSystem(IContext * pContext)
		{
			IWindow* pWindow = pContext->GetSystem<Light::IWindow>();

			int w, h;
			pWindow->VGetWindowSize(w, h);
			//glViewport(0, 0, w, h);

			pContext->VAddSystem(this);
			pContext->GetSystem<IEventManager>()->VAddListener(DEBUG_NEW EventDelegate<RenderSystem>(this, &RenderSystem::OnObjectCreate), events::EvtNewActor::StaticType);
			pContext->GetSystem<IEventManager>()->VAddListener(DEBUG_NEW EventDelegate<RenderSystem>(this, &RenderSystem::OnCameraCreate), events::EvtCameraCreate::StaticType);
			pContext->GetSystem<IEventManager>()->VAddListener(DEBUG_NEW EventDelegate<RenderSystem>(this, &RenderSystem::OnbjectDestroy), events::EvtDestroyActor::StaticType);
			pContext->GetSystem<IEventManager>()->VAddListener(DEBUG_NEW EventDelegate<RenderSystem>(this, &RenderSystem::OnSceneCreate), events::EvtSceneCreate::StaticType);


			m_DefaultPass = std::unique_ptr<RenderPass>(DEBUG_NEW DefaultRenderPass("Default", m_pContext));

		}


		void RenderSystem::Render()
		{
			// if there was no camera, so we can't see anything
			if (m_pCurrentCamera == nullptr) return;

			glm::mat4 pv = m_pCurrentCamera->GetProjMatrix()*m_pCurrentCamera->GetViewMatrix();

			{// pass default
				m_DefaultPass->Render(pv);
			}
			{// pass extra
				for (auto& pass : m_ExtraPass)
				{
					pass->Render(pv);
				}
			}

			m_pScene->VOnRender();
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

			typedef Light::render::RenderDevice* (*CreateInterfaceFn);

			CreateInterfaceFn fnCreateInterface = (CreateInterfaceFn)GetProcAddress((HMODULE)hModule, "CreateInterface");

			if (fnCreateInterface == nullptr)
			{
				E_ERROR("Can't load CreateInterface() ");
				
			}
			m_pRenderer = fnCreateInterface();

			FreeLibrary(hModule);

		
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
			if (pMeshRender == nullptr) return;

			Renderable renderable;
			renderable.m_ActorID = pActor->VGetId();
			renderable.m_pActor = pActor;
			renderable.m_RenderComponent = pMeshRender;
			renderable.m_TransformComponent = pTransform;

			pass->AddRenderObject(renderable);

			E_DEBUG("ObjectRender add: %s", pActor->VGetName().c_str());
		}
	}
}
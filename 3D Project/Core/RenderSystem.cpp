#include "pch.h"
#include "RenderSystem.h"

#include "..\Graphics3D\RenderPass\DefaultPass.h"
#include "..\Graphics3D\SkyBox.h"
#include "..\Graphics3D\Scene.h"
#include "Events.h"
#include "EventManager.h"
#include "DebugRender.h"
#include "..\Graphics3D\EffectSystem.h"
#include "..\ResourceManager\ResourceManager.h"
#include "..\Graphics3D\DefaultMesh.h"
#include "..\Graphics3D\LTModel.h"
namespace Light
{
	namespace render
	{
		RenderSystem::RenderSystem(IContext * pContext):m_pContext(pContext), m_pCurrentCamera(nullptr), m_pScene(nullptr)
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
			if (m_pCurrentCamera == nullptr|| m_pScene==nullptr) return;
			
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
		void RenderSystem::PreRender()
		{
			
		}
		Model * RenderSystem::VCreateModel(resources::ModelData * pData)
		{
			auto pResources = m_pContext->GetSystem<resources::IResourceManager>();
			auto pFactory = m_pContext->GetSystem<IFactory>();
			auto it = m_ModelList.find(pData);
			if (it != m_ModelList.end()) return it->second.get();

			if (typeid(*pData) == typeid(resources::DefaultModelData))
			{
				resources::DefaultModelData* pDefData = static_cast<resources::DefaultModelData*>(pData);
				DefaultModel* pModel = DEBUG_NEW DefaultModel;
				for (int i=0; i<pDefData->Meshs.size(); i++)
				{
					DefaultMesh* pMesh = DEBUG_NEW DefaultMesh(m_pRenderer, pDefData->Meshs[i].Vertexs, pDefData->Meshs[i].Indices, pDefData->Meshs[i].Name);
					pModel->Meshs.push_back(std::unique_ptr<DefaultMesh>(pMesh));
					pModel->Diffuse.push_back(VCreateTexture(pDefData->Texs[i]));
					pModel->Materials.push_back(std::shared_ptr<render::Material>(pFactory->VGetMaterial("Default")));
			
				}

				pModel->Box = pDefData->Box;
				pModel->MatParam = pDefData->Mats;
				m_ModelList[pData].reset(pModel);

				return pModel;
			}
			


			return nullptr;
		}
		Model * RenderSystem::VCreateModel(std::string xmlfile)
		{
			auto pResources = m_pContext->GetSystem<resources::IResourceManager>();
			auto pFactory = m_pContext->GetSystem<IFactory>();

			tinyxml2::XMLDocument doc;
			LTModel* pModelRender = nullptr;
			int errorID = doc.LoadFile(xmlfile.c_str());
			if (errorID)
			{
				E_ERROR("Failed to load XML file: %s", xmlfile.c_str());
				return nullptr;
			}
			tinyxml2::XMLElement* pData = doc.FirstChildElement();
			if (strcmp(pData->Value(), "Data") == 0)
			{
				// load model
				tinyxml2::XMLElement* pModelNode = pData->FirstChildElement("Model");
				const char* pFileName = pModelNode->Attribute("File");
				resources::LTRawData* pLTBData = static_cast<resources::LTRawData*>(pResources->VGetModel(pFileName, true));
				if (pLTBData)
				{
					pModelRender = DEBUG_NEW LTModel;

					tinyxml2::XMLElement* pTextureNode = pData->FirstChildElement("Texture");
					vector<resources::LTRawMesh>& ve = pLTBData->Meshs;
					for (size_t i = 0; i < ve.size(); i++)
					{
						pModelRender->Meshs.push_back(std::unique_ptr<Mesh>(DEBUG_NEW SkeMesh(m_pRenderer, &ve[i])));
						tinyxml2::XMLElement* pTextureElement = pTextureNode->FirstChildElement(ve[i].Name.c_str());
						if (pTextureElement)
						{
							const char* pTextureFile = pTextureElement->Attribute("File");
							pModelRender->Textures.push_back(VCreateTexture(pResources->VGetTexture(std::vector<std::string>{pTextureFile}, false,true)));
							const char* pMaterialFile = pTextureElement->Attribute("Material");
							pModelRender->Materials.push_back(m_pContext->GetSystem<IFactory>()->VGetMaterial("Skeleton"));

						}

					}
				}

				//// load material
				//tinyxml2::XMLElement* pMaterialData = pData->FirstChildElement("Material");
				//render::Material* mat;
				//tinyxml2::XMLElement* pKa = pMaterialData->FirstChildElement("Ka");
				//mat.Ka.x = pKa->FloatAttribute("r", 1.0f);
				//mat.Ka.y = pKa->FloatAttribute("g", 1.0f);
				//mat.Ka.z = pKa->FloatAttribute("b", 1.0f);
				//tinyxml2::XMLElement* pKd = pMaterialData->FirstChildElement("Kd");
				//mat.Kd.x = pKd->FloatAttribute("r", 1.0f);
				//mat.Kd.y = pKd->FloatAttribute("g", 1.0f);
				//mat.Kd.z = pKd->FloatAttribute("b", 1.0f);
				//tinyxml2::XMLElement* pKs = pMaterialData->FirstChildElement("Ks");
				//mat.Ks.x = pKs->FloatAttribute("r", 1.0f);
				//mat.Ks.y = pKs->FloatAttribute("g", 1.0f);
				//mat.Ks.z = pKs->FloatAttribute("b", 1.0f);
				//mat.exp = vec3(pKs->FloatAttribute("exp", 32.0f));
				//pModelRender->m_Material.resize(pModelRender->m_pMesh.size(), mat);
				// Done return LTModel
				m_ModelList[pLTBData].reset(pModelRender);
				return pModelRender;
			}
			return nullptr;
		}
		Texture * RenderSystem::VCreateTexture(resources::TextureData *pData)
		{

			assert(pData != nullptr);

			Texture* tex = nullptr;
			auto it = m_TextureList.find(pData);
			if (it != m_TextureList.end()) return it->second.get();

			if (pData->flag&resources::Flag_Normal|| pData->flag&resources::Flag_CubeTex)
			{
				
				tex = m_pRenderer->CreateTexture(pData->eTarget, pData->iLevel, pData->iInternalFormat, pData->uiWidth, pData->uiHeight, 0, pData->eFormat, pData->eType, pData->pData, pData->alignment);
			}
			else if (pData->flag&resources::Flag_Compress)
			{
				
				tex = m_pRenderer->CreateTexture(pData->eTarget, pData->iLevel, pData->iInternalFormat, pData->uiWidth, pData->uiHeight, 0, pData->eFormat, pData->eType, pData->pData, pData->alignment, true);
			}

			m_TextureList[pData].reset(tex);

			return tex;
		}
		Sprite * RenderSystem::VCreateSprite(resources::SpriteData *pData)
		{
			auto pResources = m_pContext->GetSystem<resources::IResourceManager>();
			auto it = m_SpriteList.find(pData);
			if (it != m_SpriteList.end()) return it->second.get();
			Sprite* p = DEBUG_NEW Sprite();

			p->m_pData = pData;
			for (auto el : pData->m_FrameLists)
			{
				auto tex = VCreateTexture(pResources->VGetTexture(std::vector<std::string>{"GameAssets\\" + el},false,true));
				p->m_FrameLists.push_back(tex);
			}
			p->m_Size = glm::vec2(p->m_FrameLists[0]->GetWidth(), p->m_FrameLists[0]->GetHeight());

			m_SpriteList[pData].reset(p);
			return p;
			
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
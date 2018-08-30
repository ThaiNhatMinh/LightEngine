#include "pch.h"
#include "OpenGLRenderDevice.h"
#include "OpenGLVertexShader.h"
#include "OpenGLPixelShader.h"
#include "OpenGLPipeline.h"
#include "OpenGLVertexBuffer.h"
#include "OpenGLVertexDescription.h"
#include "OpenGLVertexArray.h"
#include "OpenGLTexture.h"
#include "OpenGLCompressTexture.h"
#include "OpenGLIndexBuffer.h"
#include "OpenGLCubeTexture.h"
#include "OpenGLDepthStencilState.h"
#include "Core\OpenGLWindows.h"
#include "Interface\IEventManager.h"
#include "GameComponents\TransformComponent.h"
#include "GameComponents\MeshRenderComponent.h"
#include "..\..\Core\Events.h"
#include "Core\Events.h"
#include "..\..\Interface\IFactory.h"
namespace Light
{
	namespace render
	{

		static GLenum toOpenGLPrimitive[] =
		{
			GL_POINTS,
			GL_LINES,
			GL_LINE_STRIP,
			GL_LINE_LOOP,
			GL_LINES_ADJACENCY,
			GL_LINE_STRIP_ADJACENCY,
			GL_TRIANGLES,
			GL_TRIANGLE_STRIP,
			GL_TRIANGLE_FAN,
			GL_TRIANGLES_ADJACENCY,
			GL_TRIANGLE_STRIP_ADJACENCY

		};
		
		OpenGLRenderDevice::OpenGLRenderDevice(IContext * pContext):m_pCurrentCamera(nullptr)
		{
			m_pContext = pContext;
			// Initialize GLEW to setup the OpenGL Function pointers
			glewExperimental = GL_TRUE;
			GLenum err = glewInit();
			if (err != GLEW_OK)
			{
				
				E_ERROR("glewInit failed! ");
				return;
			}

			IWindow* pWindow = pContext->GetSystem<Light::IWindow>();

			int w, h;
			pWindow->VGetWindowSize(w, h);
			glViewport(0, 0, w, h);

			m_pDefaultDepthStencil = nullptr;
			render::DepthStencilConfig config;
			
			config.Depthfunc = COMPARE_LESS;
			config.DepthMask = true;
			config.FrontWriteMask = 0xFF;
			config.FrontStencilEnabled = true;
			config.FrontStencilCompare = COMPARE_NOTEQUAL;
			config.FrontRef = 1;
			config.FrontCompareMask = 0xFF;
			config.FrontStencilFail = STENCIL_KEEP;
			config.FrontDepthFail = STENCIL_KEEP;
			config.FrontStencilPass = STENCIL_REPLACE;

			

			SetDepthStencilState(DEBUG_NEW OpenGLDepthStencilState(config));


			pContext->VAddSystem(this);
			pContext->GetSystem<IEventManager>()->VAddListener(DEBUG_NEW EventDelegate<OpenGLRenderDevice>(this, &OpenGLRenderDevice::OnObjectCreate), events::EvtNewActor::StaticType);
			pContext->GetSystem<IEventManager>()->VAddListener(DEBUG_NEW EventDelegate<OpenGLRenderDevice>(this, &OpenGLRenderDevice::OnCameraCreate), events::EvtCameraCreate::StaticType);

			
			m_DefaultPass.pRenderer = this;
			m_DefaultPass.pGlobalMaterial = nullptr;
			m_DefaultPass.pDepthStencilConfig = m_pDefaultDepthStencil;
			/*glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_LESS);
			glEnable(GL_STENCIL_TEST);
			glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
			glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);*/
		}

		OpenGLRenderDevice::~OpenGLRenderDevice()
		{
			delete m_pDefaultDepthStencil;
		}

		const char * OpenGLRenderDevice::VGetName()
		{
			static const char* pName = typeid(RenderDevice).name();
			return pName;
		}

		VertexShader* OpenGLRenderDevice::CreateVertexShader(const char * code)
		{
			return DEBUG_NEW OpenGLVertexShader(code);
		}

		PixelShader * OpenGLRenderDevice::CreatePixelShader(const char * code)
		{
			return DEBUG_NEW OpenGLPixelShader(code);
		}

		Pipeline * OpenGLRenderDevice::CreatePipeline(VertexShader *pVertexShader, PixelShader *pPixelShader)
		{
			return DEBUG_NEW OpenGLPipeline(pVertexShader, pPixelShader);
		}

		VertexBuffer * OpenGLRenderDevice::CreateVertexBuffer(long long size, const void * data)
		{
			return DEBUG_NEW OpenGLVertexBuffer(size, data);
		}

		VertexDescription * OpenGLRenderDevice::CreateVertexDescription(unsigned int numElement, const VertexElement * pElement)
		{
			return DEBUG_NEW OpenGLVertexDescription(numElement, pElement);
		}

		VertexArray * OpenGLRenderDevice::CreateVertexArray(unsigned int numBuffer, VertexBuffer ** vertexBuffer, VertexDescription ** vertexDescription)
		{
			return DEBUG_NEW OpenGLVertexArray(numBuffer, vertexBuffer, vertexDescription);
		}

		Texture * OpenGLRenderDevice::CreateTexture(const TextureCreateInfo & info, bool isCompress)
		{
			if (isCompress) return DEBUG_NEW OpenGLCompressTexture(info);
			
			
			if(info.eTarget==GL_TEXTURE_2D) return DEBUG_NEW OpenGLTexture(info);
			else if (info.eTarget == GL_TEXTURE_CUBE_MAP) return DEBUG_NEW OpenGLCubeTexture(info);
			else E_WARNING("Invaild target texture: %d", info.eTarget);
			
			return nullptr;
		}

		IndexBuffer * OpenGLRenderDevice::CreateIndexBuffer(unsigned int size, const void* pData)
		{
			return DEBUG_NEW OpenGLIndexBuffer(size, pData);
		}

		DepthStencilState * OpenGLRenderDevice::CreateDepthStencilState(const DepthStencilConfig & config)
		{
			return DEBUG_NEW OpenGLDepthStencilState(config);
		}

		void OpenGLRenderDevice::SetVertexArray(VertexArray * pVertexArray)
		{
			assert(pVertexArray != nullptr);
			glBindVertexArray(static_cast<OpenGLVertexArray*>(pVertexArray)->m_iHandle);
		}

		void OpenGLRenderDevice::SetPipeline(Pipeline * pPipeline)
		{
			assert(pPipeline != nullptr);
			glUseProgram(static_cast<OpenGLPipeline*>(pPipeline)->m_iProgram);
		}

		void OpenGLRenderDevice::SetIndexBuffer(IndexBuffer *pIndexBuffer)
		{
			assert(pIndexBuffer != nullptr);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, static_cast<OpenGLIndexBuffer*>(pIndexBuffer)->m_iHandle);
		}

		void OpenGLRenderDevice::SetTexture(unsigned int slot, Texture *pTexture)
		{
			assert(pTexture != nullptr);
			glActiveTexture(GL_TEXTURE0 + slot);
			glBindTexture(pTexture->m_TexInfo.eTarget, static_cast<OpenGLTexture*>(pTexture)->m_iHandle);
		}

		void OpenGLRenderDevice::SetDepthStencilState(DepthStencilState * state)
		{
			//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			auto oldState = m_pDefaultDepthStencil;
			if (state) m_pDefaultDepthStencil = static_cast<OpenGLDepthStencilState*>(state);

			if (oldState != m_pDefaultDepthStencil)
			{
				if (m_pDefaultDepthStencil->DepthEnable)
				{
					
					glEnable(GL_DEPTH_TEST);
					//glFront(GL_CW);
					glDepthMask(m_pDefaultDepthStencil->DepthMask);
					glDepthFunc(m_pDefaultDepthStencil->Depthfunc);
				}
				if (m_pDefaultDepthStencil->FrontStencilEnabled || m_pDefaultDepthStencil->BackStencilEnabled)
				{
					glEnable(GL_STENCIL_TEST);
					glStencilFunc( m_pDefaultDepthStencil->FrontStencilCompare, m_pDefaultDepthStencil->FrontRef, m_pDefaultDepthStencil->FrontCompareMask);
					glStencilOp(m_pDefaultDepthStencil->FrontStencilFail, m_pDefaultDepthStencil->FrontDepthFail, m_pDefaultDepthStencil->FrontStencilPass);
					glStencilMask(m_pDefaultDepthStencil->FrontWriteMask);

					/*glStencilFuncSeparate(GL_BACK, m_pDefaultDepthStencil->BackStencilCompare, m_pDefaultDepthStencil->BackRef, m_pDefaultDepthStencil->BackCompareMask);
					glStencilOpSeparate(GL_BACK, m_pDefaultDepthStencil->BackStencilFail, m_pDefaultDepthStencil->BackDepthFail, m_pDefaultDepthStencil->BackStencilPass);
					glStencilMaskSeparate(GL_BACK, m_pDefaultDepthStencil->BackWriteMask);*/

				}
			}

		}

		void OpenGLRenderDevice::Clear(float r, float g, float b, float alpha, float depth)
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
			glClearColor(r, g, b, alpha);
			glClearDepth(depth);
		}

		void OpenGLRenderDevice::Draw(int first, int count, int primcount, Primitive primitive)
		{

			if (primcount) glDrawArraysInstanced(toOpenGLPrimitive[primitive], first, count, primcount);
			else glDrawArrays(toOpenGLPrimitive[primitive], first, count);

		}

		void OpenGLRenderDevice::DrawElement(int count, const void * indices, int primcount, Primitive primitive)
		{

			if (primcount) glDrawElementsInstanced(toOpenGLPrimitive[primitive], count, GL_UNSIGNED_INT, indices, primcount);
			else glDrawElements(toOpenGLPrimitive[primitive], count, GL_UNSIGNED_INT, indices);
		}

		

		void OpenGLRenderDevice::Render()
		{
			this->Clear();

			// if there was no camera, so we can't see anything
			if (m_pCurrentCamera == nullptr) return;

			glm::mat4 pv = m_pCurrentCamera->GetProjMatrix()*m_pCurrentCamera->GetViewMatrix();
			{// pass 1
				this->SetDepthStencilState(pass1.pDepthStencilConfig);
				for (auto renderable : m_ObjectRenders)
				{
					render::Model* modelRender = renderable.m_RenderComponent->m_pModel;
					IActor* actor = renderable.m_pActor;
					// computer transformation matrix
					glm::mat4 model = actor->VGetGlobalTransform();
					//model = glm::rotate(glm::mat4(), glm::radians(angle), glm::vec3(0, 1, 0));
					// just draw it
					modelRender->Draw(&m_DefaultPass, glm::value_ptr(model), glm::value_ptr(pv*model));

				}
			}
			{// pass 2
				this->SetDepthStencilState(pass2.pDepthStencilConfig);
				for (auto renderable : m_ObjectRenders)
				{
					render::Model* modelRender = renderable.m_RenderComponent->m_pModel;
					IActor* actor = renderable.m_pActor;
					// computer transformation matrix
					glm::mat4 model = actor->VGetGlobalTransform();
					//model = glm::rotate(glm::mat4(), glm::radians(angle), glm::vec3(0, 1, 0));
					// just draw it
					modelRender->Draw(&pass2, glm::value_ptr(model), glm::value_ptr(pv*model));

				}
			}

			SetDepthStencilState(m_DefaultPass.pDepthStencilConfig);
		}

		render::ICamera * OpenGLRenderDevice::VGetCurrentCamera()
		{
			return m_pCurrentCamera;
		}

		void OpenGLRenderDevice::VSetCurrentCamera(render::ICamera * cam)
		{
			m_pCurrentCamera = cam;
		}

		void OpenGLRenderDevice::Test()
		{
			auto Resources = m_pContext->GetSystem<resources::IResourceManager>();
			auto VS = Resources->VGetVertexShader("Outline");
			auto FS = Resources->VGetPixelShader("Color");
			auto pipeline = this->CreatePipeline(VS, FS);
			{
				render::DepthStencilConfig config;
				config.FrontStencilEnabled = true;
				config.FrontWriteMask = 0xFF;
				config.FrontStencilCompare = render::COMPARE_ALWAYS;
				config.FrontRef = 1;
				config.FrontCompareMask = 0xFF;
				DepthStencilState* DSS = DEBUG_NEW OpenGLDepthStencilState(config);
				pass1.pRenderer = this;
				pass1.pDepthStencilConfig = DSS;
				
			}
			{
				render::DepthStencilConfig config;
				config.FrontStencilEnabled = true;
				config.FrontStencilCompare = render::COMPARE_NOTEQUAL;
				config.FrontRef = 1;
				config.FrontCompareMask = 0xFF;
				config.FrontWriteMask = 0x00;
				config.DepthEnable = false;

				DepthStencilState* DSS = DEBUG_NEW OpenGLDepthStencilState(config);
				pass2.pRenderer = this;
				pass2.pDepthStencilConfig = DSS;
				pass2.pGlobalMaterial = m_pContext->GetSystem<IFactory>()->VGetMaterial("Default")->Clone();
				pass2.pGlobalMaterial->SetPipeline(pipeline);

			}
		}

		void OpenGLRenderDevice::OnObjectCreate(std::shared_ptr<IEvent> event)
		{
			IActor* pActor = static_cast<events::EvtNewActor*>(event.get())->GetActor();
			IMeshRenderComponent* pMeshRender = pActor->GetComponent<IMeshRenderComponent>();
			ITransformComponent* pTransform = pActor->GetComponent<ITransformComponent>();
			if (pMeshRender == nullptr) return;

			Renderable renderable;
			renderable.m_ActorID = pActor->VGetId();
			renderable.m_pActor = pActor;
			renderable.m_RenderComponent = pMeshRender;
			renderable.m_TransformComponent = pTransform;

			m_ObjectRenders.push_back(renderable);
			E_DEBUG("ObjectRender add: %s", pActor->VGetName().c_str());

		}

		void OpenGLRenderDevice::OnbjectDestroy(std::shared_ptr<IEvent> event)
		{
			ActorId Actor = static_cast<events::EvtDestroyActor*>(event.get())->GetActor();

			auto result = std::find_if(m_ObjectRenders.begin(), m_ObjectRenders.end(), [Actor](Renderable renderable)
			{
				return Actor == renderable.m_ActorID;
			});
			
			if (result == m_ObjectRenders.end()) return;

			m_ObjectRenders.erase(result);

		}

		void OpenGLRenderDevice::OnCameraCreate(std::shared_ptr<IEvent> event)
		{
			events::EvtCameraCreate* pEvent = static_cast<events::EvtCameraCreate*>(event.get());

			m_pCurrentCamera = pEvent->GetCamera();
			// just for debug, should remove in release
						
			E_DEBUG("Camera create!");
			
		}

	}

}

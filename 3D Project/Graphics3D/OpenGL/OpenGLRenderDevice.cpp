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
#include "OpenGLFrameBuffer.h"
#include "OpenGLRenderBuffer.h"
#include "DataMap.h"
#include "Core\OpenGLWindows.h"
#include "Interface\IEventManager.h"
#include "GameComponents\TransformComponent.h"
#include "GameComponents\MeshRenderComponent.h"
#include "..\..\Core\Events.h"
#include "Core\Events.h"
#include "..\..\Interface\IFactory.h"
#include "..\RenderPass\DefaultPass.h"
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

			{
				m_pDefaultDepthStencil = nullptr;
				render::DepthStencilConfig config;

				config.Depthfunc = COMPARE_LESS;
				config.DepthMask = true;
				config.FrontWriteMask = 0xFF;
				config.FrontStencilEnabled = true;
				config.FrontStencilCompare = COMPARE_NOTEQUAL;
				config.FrontRef = 1;
				config.FrontCompareMask = 0xFFFFFFFF;
				config.FrontStencilFail = STENCIL_KEEP;
				config.FrontDepthFail = STENCIL_KEEP;
				config.FrontStencilPass = STENCIL_REPLACE;
				m_pDefaultDepthStencil = DEBUG_NEW OpenGLDepthStencilState(config);
			}

			{
				m_pDefaultRaster = nullptr;
				render::CullFaceConfig config;
				m_pDefaultRaster = DEBUG_NEW OpenGLRasterState(config); 
				this->SetRasterState(m_pDefaultRaster);
			}

			{
				m_pDefaultBlending = nullptr;
				render::BlendConfig config;
				m_pDefaultBlending = DEBUG_NEW OpenGLBlendingState(config);
				this->SetBlendingState(m_pDefaultBlending);
			}

			pContext->VAddSystem(this);
			pContext->GetSystem<IEventManager>()->VAddListener(DEBUG_NEW EventDelegate<OpenGLRenderDevice>(this, &OpenGLRenderDevice::OnObjectCreate), events::EvtNewActor::StaticType);
			pContext->GetSystem<IEventManager>()->VAddListener(DEBUG_NEW EventDelegate<OpenGLRenderDevice>(this, &OpenGLRenderDevice::OnCameraCreate), events::EvtCameraCreate::StaticType);
			pContext->GetSystem<IEventManager>()->VAddListener(DEBUG_NEW EventDelegate<OpenGLRenderDevice>(this, &OpenGLRenderDevice::OnbjectDestroy), events::EvtDestroyActor::StaticType);


			m_DefaultPass = std::unique_ptr<RenderPass>(DEBUG_NEW DefaultRenderPass("Default", m_pContext));

		}

		OpenGLRenderDevice::~OpenGLRenderDevice()
		{
			delete m_pDefaultDepthStencil;
			delete m_pDefaultRaster;
			delete m_pDefaultBlending;
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
			
			
			if(info.eTarget==TEXTURE_2D) return DEBUG_NEW OpenGLTexture(info);
			else if (info.eTarget == TEXTURE_CUBE_MAP) return DEBUG_NEW OpenGLCubeTexture(info);
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

		RasterState * OpenGLRenderDevice::CreateRasterState(const CullFaceConfig & config)
		{
			return DEBUG_NEW OpenGLRasterState(config);
		}

		BlendingState * OpenGLRenderDevice::CreateBlendingState(const BlendConfig & config)
		{
			return DEBUG_NEW OpenGLBlendingState(config);
		}

		FrameBuffer * OpenGLRenderDevice::CreateFrameBuffer()
		{
			return DEBUG_NEW OpenGLFrameBuffer();
		}

		RenderBuffer * OpenGLRenderDevice::CreateRenderBuffer(ColorFormat format, int w, int h)
		{
			if (w == -1 || h == -1)
			{
				IWindow* pWindow = m_pContext->GetSystem<Light::IWindow>();
				pWindow->VGetWindowSize(w, h);
			}

			return DEBUG_NEW OpenGLRenderBuffer(w,h,format);
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
			glBindTexture(openGLTexType[pTexture->m_TexInfo.eTarget], static_cast<OpenGLTexture*>(pTexture)->m_iHandle);
		}

		void OpenGLRenderDevice::SetDepthStencilState(DepthStencilState * state)
		{
			//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			auto oldState = m_pCurrentDepthStencil;
			if (state) m_pCurrentDepthStencil = static_cast<OpenGLDepthStencilState*>(state);

			if (oldState != m_pCurrentDepthStencil)
			{
				if (m_pCurrentDepthStencil->DepthEnable)
				{
					
					glEnable(GL_DEPTH_TEST);
					//glFront(GL_CW);
					glDepthMask(m_pCurrentDepthStencil->DepthMask);
					glDepthFunc(m_pCurrentDepthStencil->Depthfunc);
				}
				else glDisable(GL_DEPTH_TEST);

				if (m_pCurrentDepthStencil->FrontStencilEnabled || m_pCurrentDepthStencil->BackStencilEnabled)
				{
					glEnable(GL_STENCIL_TEST);
					glStencilFunc( m_pCurrentDepthStencil->FrontStencilCompare, m_pCurrentDepthStencil->FrontRef, m_pCurrentDepthStencil->FrontCompareMask);
					glStencilOp(m_pCurrentDepthStencil->FrontStencilFail, m_pCurrentDepthStencil->FrontDepthFail, m_pCurrentDepthStencil->FrontStencilPass);
					glStencilMask(m_pCurrentDepthStencil->FrontWriteMask);

					/*glStencilFuncSeparate(GL_BACK, m_pCurrentDepthStencil->BackStencilCompare, m_pCurrentDepthStencil->BackRef, m_pCurrentDepthStencil->BackCompareMask);
					glStencilOpSeparate(GL_BACK, m_pCurrentDepthStencil->BackStencilFail, m_pCurrentDepthStencil->BackDepthFail, m_pCurrentDepthStencil->BackStencilPass);
					glStencilMaskSeparate(GL_BACK, m_pCurrentDepthStencil->BackWriteMask);*/

				}
				else glDisable(GL_STENCIL_TEST);

			}

		}

		void OpenGLRenderDevice::SetRasterState(RasterState * state)
		{
			auto oldState = m_pCurrentRaster;

			if (state) m_pCurrentRaster = static_cast<OpenGLRasterState*>(state);
			else m_pCurrentRaster = m_pDefaultRaster;

			if (m_pCurrentRaster != oldState)
			{
				if (m_pCurrentRaster->CullEnable)
				{
					glEnable(GL_CULL_FACE);
					glFrontFace(m_pCurrentRaster->FrontFaceWinding);
					glCullFace(m_pCurrentRaster->CullFace);
					glPolygonMode(GL_FRONT_AND_BACK, m_pCurrentRaster->FillMode);
				}
				else glDisable(GL_CULL_FACE);
			}

		}

		void OpenGLRenderDevice::SetBlendingState(BlendingState * state)
		{
			auto oldState = m_pCurrentBlending;

			if (state) m_pCurrentBlending = static_cast<OpenGLBlendingState*>(state);
			else m_pCurrentBlending = m_pDefaultBlending;

			if (m_pCurrentBlending != oldState)
			{
				if (m_pCurrentBlending->Enable)
				{
					glEnable(GL_BLEND);
					glBlendFunc(m_pCurrentBlending->sfactor, m_pCurrentBlending->dfactor);
					glBlendEquation(m_pCurrentBlending->func);
				}
				else glDisable(GL_BLEND);
			}
		}

		void OpenGLRenderDevice::Clear(float r, float g, float b, float alpha, float depth)
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
			glClearColor(r, g, b, alpha);
			glClearDepth(depth);
			glClearStencil(0);
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

			{// pass default
				m_DefaultPass->Render(pv);
			}
			{// pass extra
				for (auto& pass : m_ExtraPass)
				{
					pass->Render(pv);
				}
			}
		}

		render::ICamera * OpenGLRenderDevice::VGetCurrentCamera()
		{
			return m_pCurrentCamera;
		}

		void OpenGLRenderDevice::VSetCurrentCamera(render::ICamera * cam)
		{
			m_pCurrentCamera = cam;
		}

		void OpenGLRenderDevice::AddExtraPass(RenderPass* pass)
		{
			auto r = std::find_if(m_ExtraPass.begin(), m_ExtraPass.end(), [pass](const std::unique_ptr<RenderPass>& renderPass)
			{
				return renderPass->GetName() == pass->GetName();
			});

			if (r != m_ExtraPass.end()) return;

			m_ExtraPass.push_back(std::unique_ptr<RenderPass>(pass));

		}

		RenderPass * OpenGLRenderDevice::GetRenderPass(const std::string & name)
		{
			if (name == "Default") return m_DefaultPass.get();

			auto r = std::find_if(m_ExtraPass.begin(), m_ExtraPass.end(), [name](const std::unique_ptr<RenderPass>& renderPass)
			{
				return renderPass->GetName() == name;
			});

			if (r == m_ExtraPass.end()) return nullptr;

			return (*r).get();
		}

		/*void OpenGLRenderDevice::Test()
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
		}*/

		void OpenGLRenderDevice::OnObjectCreate(std::shared_ptr<IEvent> event)
		{
			
			IActor* pActor = static_cast<events::EvtNewActor*>(event.get())->GetActor();

			AddObjectToPass(pActor, m_DefaultPass.get());
		
			for (auto& pass : m_ExtraPass)
				AddObjectToPass(pActor, pass.get());

		}

		void OpenGLRenderDevice::OnbjectDestroy(std::shared_ptr<IEvent> event)
		{
			ActorId Actor = static_cast<events::EvtDestroyActor*>(event.get())->GetActor();

			m_DefaultPass->RemoveRenderObject(Actor);

			E_DEBUG("ObjectRender destroy: %d", Actor);

		}

		void OpenGLRenderDevice::OnCameraCreate(std::shared_ptr<IEvent> event)
		{
			events::EvtCameraCreate* pEvent = static_cast<events::EvtCameraCreate*>(event.get());

			m_pCurrentCamera = pEvent->GetCamera();
			// just for debug, should remove in release
						
			E_DEBUG("Camera create!");
			
		}

		void OpenGLRenderDevice::AddObjectToPass(IActor * pActor, RenderPass * pass)
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

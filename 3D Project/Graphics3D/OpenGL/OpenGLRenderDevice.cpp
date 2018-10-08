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
#include "OpenGLStencilState.h"
#include "OpenGLDepthState.h"
#include "OpenGLFrameBuffer.h"
#include "OpenGLRenderBuffer.h"
#include "DataMap.h"

#include "Interface\IEventManager.h"
#include "GameComponents\TransformComponent.h"
#include "GameComponents\MeshRenderComponent.h"
#include "..\..\Core\Events.h"
#include "Core\Events.h"
#include "..\..\Interface\IFactory.h"
#include "..\..\Interface\IWindow.h"

#include "..\RenderPass\DefaultPass.h"
#include "..\SkyBox.h"
#include "..\Scene.h"


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
		
		OpenGLRenderDevice::OpenGLRenderDevice(IContext * pContext):m_pCurrentCamera(nullptr), m_pScene(nullptr)
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
				m_pDefaultStencil = nullptr;
				render::StencilConfig config;
				config.FrontWriteMask = 0xFF;
				config.FrontEnabled = true;
				config.FrontCompare = COMPARE_NOTEQUAL;
				config.FrontRef = 1;
				config.FrontCompareMask = 0xFFFFFFFF;
				config.FrontStencilFail = STENCIL_KEEP;
				config.FrontDepthFail = STENCIL_KEEP;
				config.FrontPass = STENCIL_KEEP;
				m_pDefaultStencil = DEBUG_NEW OpenGLStencilState(config);
				this->SetStencilState(m_pDefaultStencil);
			}
			{
				m_pDefaultDepth = DEBUG_NEW OpenGLDepthState(true, true, COMPARE_LESS);
				this->SetDepthState(m_pDefaultDepth);
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
			pContext->GetSystem<IEventManager>()->VAddListener(DEBUG_NEW EventDelegate<OpenGLRenderDevice>(this, &OpenGLRenderDevice::OnSceneCreate), events::EvtSceneCreate::StaticType);


			m_DefaultPass = std::unique_ptr<RenderPass>(DEBUG_NEW DefaultRenderPass("Default", m_pContext));

		}

		OpenGLRenderDevice::~OpenGLRenderDevice()
		{
			delete m_pDefaultStencil;
			delete m_pDefaultDepth;
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

		DepthState * OpenGLRenderDevice::CreateDepthState(bool enable, bool mask, Compare depthFunc)
		{
			return DEBUG_NEW OpenGLDepthState(enable,mask,depthFunc);
		}
		
		StencilState * OpenGLRenderDevice::CreateStencilState(const StencilConfig & config)
		{
			return DEBUG_NEW OpenGLStencilState(config);
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

		void OpenGLRenderDevice::SetTexture(TextureUnit slot, Texture *pTexture)
		{
			assert(pTexture != nullptr);
			glActiveTexture(openglTexuint[slot]);
			glBindTexture(openGLTexType[pTexture->m_TexInfo.eTarget], static_cast<OpenGLTexture*>(pTexture)->m_iHandle);
		}

		void OpenGLRenderDevice::SetStencilState(StencilState * state)
		{
			//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			auto oldState = m_pCurrentStencil;
			if (state) m_pCurrentStencil = static_cast<OpenGLStencilState*>(state);

			if (oldState != m_pCurrentStencil)
			{
				

				if (m_pCurrentStencil->FrontStencilEnabled || m_pCurrentStencil->BackStencilEnabled) glEnable(GL_STENCIL_TEST);
				else glDisable(GL_STENCIL_TEST);

				glStencilFuncSeparate(GL_FRONT, m_pCurrentStencil->FrontStencilCompare, m_pCurrentStencil->FrontRef, m_pCurrentStencil->FrontCompareMask);
				glStencilOpSeparate(GL_FRONT, m_pCurrentStencil->FrontStencilFail, m_pCurrentStencil->FrontDepthFail, m_pCurrentStencil->FrontStencilPass);
				glStencilMaskSeparate(GL_FRONT, m_pCurrentStencil->FrontWriteMask);
				
				glStencilFuncSeparate(GL_BACK, m_pCurrentStencil->BackStencilCompare, m_pCurrentStencil->BackRef, m_pCurrentStencil->BackCompareMask);
				glStencilOpSeparate(GL_BACK, m_pCurrentStencil->BackStencilFail, m_pCurrentStencil->BackDepthFail, m_pCurrentStencil->BackStencilPass);
				glStencilMaskSeparate(GL_BACK, m_pCurrentStencil->BackWriteMask);

			}

		}

		void OpenGLRenderDevice::SetDepthState(DepthState * state)
		{
			//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			auto oldState = m_pCurrentDepth;
			if (state) m_pCurrentDepth = static_cast<OpenGLDepthState*>(state);

			if (oldState != m_pCurrentDepth)
			{
				if (m_pCurrentDepth->DepthEnable) glEnable(GL_DEPTH_TEST);
				else glDisable(GL_DEPTH_TEST);

				glDepthMask(m_pCurrentDepth->DepthMask);
				glDepthFunc(m_pCurrentDepth->Depthfunc);
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

		void OpenGLRenderDevice::SetDepth(bool enable, bool mask, Compare depthFunc)
		{
			if (enable) glEnable(GL_DEPTH_TEST);
			else glDisable(GL_DEPTH_TEST);
			glDepthMask(mask);
			glDepthFunc(DepthFunc[depthFunc]);
		}

		void OpenGLRenderDevice::SetStencil(bool enable, Compare compare, StencilAction StencilFail, StencilAction depthFail, StencilAction depthstencilpass, int ref, unsigned int comparemask, unsigned int writemask)
		{
			if (enable) glEnable(GL_STENCIL_TEST);
			else glDisable(GL_STENCIL_TEST);

			glStencilFunc(DepthFunc[compare],ref, comparemask);
			glStencilOp(StencilOp[StencilFail], StencilOp[depthFail], StencilOp[depthstencilpass]);
			glStencilMask(writemask);
		}

		void OpenGLRenderDevice::SetCullFace(bool enable, Face cullFace, Winding frontWindinng, RasterMode fillmode)
		{
			if (enable)
			{
				glEnable(GL_CULL_FACE);
				glFrontFace(openGLWiding[frontWindinng]);
				glCullFace(openGLFace[cullFace]);
				glPolygonMode(GL_FRONT_AND_BACK, openGLFill[fillmode]);
			}
			else glDisable(GL_CULL_FACE);
		}

		void OpenGLRenderDevice::SetBlend(bool enable, BlendFactor sfactor, BlendFactor dfactor, BlendFunc func)
		{
			if (enable)
			{
				glEnable(GL_BLEND);
				glBlendFunc(openGLfactor[sfactor], openGLfactor[dfactor]);
				glBlendEquation(openGLfunc[func]);
			}
			else glDisable(GL_BLEND);
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

		Texture * OpenGLRenderDevice::GetSkyBoxTexture()
		{
			SkyBox* pSky = static_cast<Scene*>(m_pScene)->GetSkyBox();
			return pSky->GetTexture();
			
		}

		IScene * OpenGLRenderDevice::GetScene()
		{
			return m_pScene;
		}

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

		void OpenGLRenderDevice::OnSceneCreate(std::shared_ptr<IEvent> event)
		{
			events::EvtSceneCreate* pEvent = static_cast<events::EvtSceneCreate*>(event.get());
			m_pScene = pEvent->m_pScene;
			E_DEBUG("SceneRender add: %s", m_pScene->VGetSceneName().c_str());
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

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
#include "Core\Events.h"
#include "GameComponents\TransformComponent.h"
#include "GameComponents\MeshRenderComponent.h"

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

		OpenGLRenderDevice::OpenGLRenderDevice(IContext * pContext)
		{
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

			m_pDefaultDepthStencil = new OpenGLDepthStencilState(DepthStencilConfig());
			pContext->VAddSystem(this);
			pContext->GetSystem<IEventManager>()->VAddListener(new EventDelegate<OpenGLRenderDevice>(this, &OpenGLRenderDevice::OnObjectCreate), events::EvtNewActor::StaticType);
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
			return new OpenGLVertexShader(code);
		}

		PixelShader * OpenGLRenderDevice::CreatePixelShader(const char * code)
		{
			return new OpenGLPixelShader(code);
		}

		Pipeline * OpenGLRenderDevice::CreatePipeline(VertexShader *pVertexShader, PixelShader *pPixelShader)
		{
			return new OpenGLPipeline(pVertexShader, pPixelShader);
		}

		VertexBuffer * OpenGLRenderDevice::CreateVertexBuffer(long long size, const void * data)
		{
			return new OpenGLVertexBuffer(size, data);
		}

		VertexDescription * OpenGLRenderDevice::CreateVertexDescription(unsigned int numElement, const VertexElement * pElement)
		{
			return new OpenGLVertexDescription(numElement, pElement);
		}

		VertexArray * OpenGLRenderDevice::CreateVertexArray(unsigned int numBuffer, VertexBuffer ** vertexBuffer, VertexDescription ** vertexDescription)
		{
			return new OpenGLVertexArray(numBuffer, vertexBuffer, vertexDescription);
		}

		Texture * OpenGLRenderDevice::CreateTexture(const TextureCreateInfo & info, bool isCompress)
		{
			if (isCompress) return new OpenGLCompressTexture(info);
			
			
			if(info.eTarget==GL_TEXTURE_2D) return new OpenGLTexture(info);
			else if (info.eTarget == GL_TEXTURE_CUBE_MAP) return new OpenGLCubeTexture(info);
			else E_WARNING("Invaild target texture: %d", info.eTarget);
			
			return nullptr;
		}

		IndexBuffer * OpenGLRenderDevice::CreateIndexBuffer(unsigned int size, const void* pData)
		{
			return new OpenGLIndexBuffer(size, pData);
		}

		DepthStencilState * OpenGLRenderDevice::CreateDepthStencilState(const DepthStencilConfig & config)
		{
			return new OpenGLDepthStencilState(config);
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
			auto oldState = m_pDefaultDepthStencil;
			if (state) m_pDefaultDepthStencil = static_cast<OpenGLDepthStencilState*>(state);

		}

		void OpenGLRenderDevice::Clear(float r, float g, float b, float alpha, float depth)
		{
			glClear(GL_COLOR_BUFFER_BIT);
			glClearColor(r, g, b, alpha);
			glClearDepth(depth);
		}

		void OpenGLRenderDevice::Draw(int first, int count, int primcount, Primitive primitive)
		{

			if (primcount) glDrawArraysInstanced(toOpenGLPrimitive[primitive], first, count, primcount);
			else glDrawArrays(toOpenGLPrimitive[primitive], first, count);

		}

		void OpenGLRenderDevice::DrawElement(int count, int type, const void * indices, int primcount, Primitive primitive)
		{
			if (primcount) glDrawElementsInstanced(primitive, count, type, indices, primcount);
			else glDrawElements(primitive, count, type, indices);
		}

		void OpenGLRenderDevice::Render()
		{
			for (auto renderable : m_ObjectRenders)
			{
				render::Model* modelRender = renderable.m_RenderComponent->m_pModel;
				//for(auto mesh)
			}
		}

		void OpenGLRenderDevice::OnObjectCreate(std::shared_ptr<IEvent> event)
		{
			IActor* pActor = static_cast<events::EvtNewActor*>(event.get())->GetActor();
			IMeshRenderComponent* pMeshRender = pActor->GetComponent<IMeshRenderComponent>();
			ITransformComponent* pTransform = pActor->GetComponent<ITransformComponent>();
			if (pMeshRender == nullptr) return;

			Renderable renderable;
			renderable.m_Actor = pActor->VGetId();
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
				return Actor == renderable.m_Actor;
			});
			
			if (result == m_ObjectRenders.end()) return;

			m_ObjectRenders.erase(result);

		}

		void OpenGLRenderDevice::OnCameraCreate(std::shared_ptr<IEvent> event)
		{
		}

	}

}

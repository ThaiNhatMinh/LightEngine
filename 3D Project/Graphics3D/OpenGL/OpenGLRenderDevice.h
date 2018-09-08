#pragma once

#include "..\Renderer.h"
#include "..\..\Interface\IContext.h"
#include "Interface\IEvent.h"
#include "OpenGLDepthStencilState.h"
#include "..\..\Interface\IComponent.h"
#include "..\..\GameComponents\CameraComponent.h"
#include "..\RenderPass.h"
namespace Light
{
	namespace render
	{
		class OpenGLRenderDevice : public RenderDevice
		{
		public:
			OpenGLRenderDevice(IContext* pContext);
			~OpenGLRenderDevice();
			virtual const char*			VGetName()override;

			virtual VertexShader*		CreateVertexShader(const char* code)override;
			virtual PixelShader*		CreatePixelShader(const char* code)override;
			virtual Pipeline*			CreatePipeline(VertexShader*, PixelShader*)override;
			virtual VertexBuffer*		CreateVertexBuffer(long long size, const void* data = nullptr)override;
			virtual VertexDescription*	CreateVertexDescription(unsigned int numElement, const VertexElement* pElement)override;
			virtual VertexArray*		CreateVertexArray(unsigned int numBuffer, VertexBuffer**vertexBuffer, VertexDescription** vertexDescription)override;
			virtual Texture*			CreateTexture(const TextureCreateInfo& info, bool isCompress = false)override;
			virtual IndexBuffer*		CreateIndexBuffer(unsigned int size, const void* pData = nullptr)override;
			virtual DepthStencilState*	CreateDepthStencilState(const DepthStencilConfig& config);

			virtual void				SetVertexArray(VertexArray*)override;
			virtual void				SetPipeline(Pipeline*)override;
			virtual void				SetIndexBuffer(IndexBuffer*)override;
			virtual void				SetTexture(unsigned int slot, Texture*)override;
			virtual void				SetDepthStencilState(DepthStencilState* state = nullptr);

			virtual void				Clear(float r = 0.0f, float g = 0.0f, float b = 0.0f, float alpha = 1.0f, float depth = 1.0f)override;
			virtual void				Draw(int first, int count, int primcount = 0, Primitive primitive = PRIMITIVE_TRIANGLES)override;
			virtual void				DrawElement(int count, const void * indices, int primcount = 0, Primitive primitive = PRIMITIVE_TRIANGLES)override;
			virtual void				Render();

			virtual render::ICamera*	VGetCurrentCamera()override;
			virtual void				VSetCurrentCamera(render::ICamera * cam) override;
			virtual void				AddExtraPass(RenderPass* pass);
			virtual RenderPass*			GetRenderPass(const std::string& name = "Default")override;
		private:

			

			//using RenderableList = std::list<Renderable>;

			void OnObjectCreate(std::shared_ptr<IEvent> event);
			void OnbjectDestroy(std::shared_ptr<IEvent> event);
			void OnCameraCreate(std::shared_ptr<IEvent> event);

			void AddObjectToPass(IActor* pActor, RenderPass* pass);

		private:
			ICamera * m_pCurrentCamera;
			//RenderableList m_ObjectRenders;
			IContext * m_pContext;
			OpenGLDepthStencilState* m_pDefaultDepthStencil;
			OpenGLDepthStencilState* m_pCurrentDepthStencil;

			std::unique_ptr<RenderPass> m_DefaultPass;
			std::list<std::unique_ptr<RenderPass>> m_ExtraPass;

		};
	}
}


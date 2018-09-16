#pragma once

#include "..\Renderer.h"
#include "..\..\Interface\IContext.h"
#include "Interface\IEvent.h"
#include "OpenGLDepthStencilState.h"
#include "OpenGLRasterState.h"
#include "OpenGLBlendingState.h"

#include "..\..\Interface\IComponent.h"
#include "..\..\GameComponents\CameraComponent.h"
#include "..\RenderPass.h"
#include "..\..\Interface\IScene.h"

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
			virtual RasterState*		CreateRasterState(const CullFaceConfig& config) override;
			virtual BlendingState*		CreateBlendingState(const BlendConfig& config)override;
			virtual FrameBuffer*		CreateFrameBuffer()override;
			virtual RenderBuffer*		CreateRenderBuffer(ColorFormat format, int w = -1, int h = -1)override;


			virtual void				SetVertexArray(VertexArray*)override;
			virtual void				SetPipeline(Pipeline*)override;
			virtual void				SetIndexBuffer(IndexBuffer*)override;
			virtual void				SetTexture(TextureUnit slot, Texture*)override;
			virtual void				SetDepthStencilState(DepthStencilState* state = nullptr);
			virtual void				SetRasterState(RasterState* state = nullptr)override;
			virtual void				SetBlendingState(BlendingState* state = nullptr)override;


			virtual void				Clear(float red = 0.2f, float green = 0.2f, float blue = 0.2f, float alpha = 1.0f, float depth = 1.0f)override;
			virtual void				Draw(int first, int count, int primcount = 0, Primitive primitive = PRIMITIVE_TRIANGLES)override;
			virtual void				DrawElement(int count, const void * indices, int primcount = 0, Primitive primitive = PRIMITIVE_TRIANGLES)override;
			virtual void				Render();

			virtual render::ICamera*	VGetCurrentCamera()override;
			virtual void				VSetCurrentCamera(render::ICamera * cam) override;
			virtual void				AddExtraPass(RenderPass* pass);
			virtual RenderPass*			GetRenderPass(const std::string& name = "Default")override;
			virtual Texture*			GetSkyBoxTexture();
			virtual IScene*				GetScene();
		private:

			

			//using RenderableList = std::list<Renderable>;

			void OnObjectCreate(std::shared_ptr<IEvent> event);
			void OnbjectDestroy(std::shared_ptr<IEvent> event);
			void OnCameraCreate(std::shared_ptr<IEvent> event);
			void OnSceneCreate(std::shared_ptr<IEvent> event);
			void AddObjectToPass(IActor* pActor, RenderPass* pass);

		private:
			
			ICamera * m_pCurrentCamera;
			
			IContext * m_pContext;
			OpenGLDepthStencilState* m_pDefaultDepthStencil, *m_pCurrentDepthStencil;
			OpenGLRasterState * m_pCurrentRaster, *m_pDefaultRaster;
			OpenGLBlendingState * m_pCurrentBlending, *m_pDefaultBlending;

			std::unique_ptr<RenderPass> m_DefaultPass;
			std::list<std::unique_ptr<RenderPass>> m_ExtraPass;
			IScene* m_pScene;
		};
	}
}


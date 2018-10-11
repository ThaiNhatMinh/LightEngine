#pragma once

#include <Renderer.h>

#include "OpenGLRasterState.h"
#include "OpenGLBlendingState.h"
#include "OpenGLDepthState.h"
#include "OpenGLStencilState.h"
namespace Light
{
	namespace render
	{
		class OpenGLRenderDevice : public RenderDevice
		{
		public:
			OpenGLRenderDevice();
			~OpenGLRenderDevice();
			
			virtual void				SetViewPort(size_t x, size_t y, size_t width, size_t height) override;
			virtual VertexShader*		CreateVertexShader(const char* code)override;
			virtual PixelShader*		CreatePixelShader(const char* code)override;
			virtual Pipeline*			CreatePipeline(VertexShader*, PixelShader*)override;
			virtual VertexBuffer*		CreateVertexBuffer(long long size, const void* data = nullptr)override;
			virtual VertexDescription*	CreateVertexDescription(unsigned int numElement, const VertexElement* pElement)override;
			virtual VertexArray*		CreateVertexArray(unsigned int numBuffer, VertexBuffer**vertexBuffer, VertexDescription** vertexDescription)override;
			virtual Texture*			CreateTexture(const TextureCreateInfo& info, bool isCompress = false)override;
			virtual IndexBuffer*		CreateIndexBuffer(unsigned int size, const void* pData = nullptr)override;
			virtual DepthState*			CreateDepthState(bool enable = true, bool mask = true, Compare depthFunc = COMPARE_LESS) override;
			virtual StencilState*		CreateStencilState(const StencilConfig & config) override;
			virtual RasterState*		CreateRasterState(const CullFaceConfig& config) override;
			virtual BlendingState*		CreateBlendingState(const BlendConfig& config)override;
			virtual FrameBuffer*		CreateFrameBuffer()override;
			virtual RenderBuffer*		CreateRenderBuffer(ColorFormat format, int w = -1, int h = -1)override;


			virtual void				SetVertexArray(VertexArray*)override;
			virtual void				SetPipeline(Pipeline*)override;
			virtual void				SetIndexBuffer(IndexBuffer*)override;
			virtual void				SetTexture(TextureUnit slot, Texture*)override;
			virtual void				SetDepthState(DepthState* state = nullptr) override;
			virtual void				SetStencilState(StencilState* state = nullptr) override;
			virtual void				SetRasterState(RasterState* state = nullptr)override;
			virtual void				SetBlendingState(BlendingState* state = nullptr)override;
			// immediate apply
			virtual void				SetDepth(bool enable = true, bool mask = true, Compare depthFunc = COMPARE_LESS) override;
			virtual void				SetStencil(bool enable = false, Compare compare = COMPARE_NOTEQUAL, StencilAction StencilFail = STENCIL_KEEP,
				StencilAction depthFail = STENCIL_KEEP, StencilAction depthstencilpass = STENCIL_KEEP,
				int ref = 1, unsigned int comparemask = 0xFFFFFFFF, unsigned int writemask = 0xFFFFFFFF) override;
			virtual void				SetCullFace(bool enable = true, Face cullFace = FACE_BACK, Winding frontWindinng = WINDING_CCW, RasterMode fillmode = RASTERMODE_FILL) override;
			virtual void				SetBlend(bool enable = false, BlendFactor sfactor = FACTOR_SRC_ALPHA, BlendFactor dfactor = FACTOR_ONE_MINUS_SRC_ALPHA, BlendFunc func = FUNC_ADD) override;


			virtual void				Clear(float red = 0.2f, float green = 0.2f, float blue = 0.2f, float alpha = 1.0f, float depth = 1.0f)override;
			virtual void				Draw(int first, int count, int primcount = 0, Primitive primitive = PRIMITIVE_TRIANGLES)override;
			virtual void				DrawElement(int count, const void * indices, int primcount = 0, Primitive primitive = PRIMITIVE_TRIANGLES)override;
			

			
	

		private:
			
			
			OpenGLDepthState* m_pDefaultDepth, *m_pCurrentDepth;
			OpenGLRasterState * m_pCurrentRaster, *m_pDefaultRaster;
			OpenGLBlendingState * m_pCurrentBlending, *m_pDefaultBlending;
			OpenGLStencilState* m_pDefaultStencil, *m_pCurrentStencil;
			
		};
	}
}


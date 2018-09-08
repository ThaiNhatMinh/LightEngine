#pragma once

#include "ICamera.h"
#include "RenderPass.h"
namespace Light
{
	class IActor;
	namespace render
	{
		struct TextureCreateInfo
		{
			unsigned int eTarget;		//  GL_TEXTURE_2D, GL_PROXY_TEXTURE_2D, GL_TEXTURE_1D_ARRAY, GL_PROXY_TEXTURE_1D_ARRAY, GL_TEXTURE_RECTANGLE, GL_PROXY_TEXTURE_RECTANGLE
			int iLevel;				// Specifies the level-of-detail number.
			int iInternalFormat;		// Specifies the number of color components in the texture.
			unsigned int uiWidth, uiHeight;	// Specifies the width/height of the texture image
			unsigned int eFormat;				// Specifies the format of the pixel data.
			unsigned int eType;				// Specifies the data type of the pixel data
			void* pData;				// Specifies a pointer to the image data in memory. 
			TextureCreateInfo() {};
		};

		class VertexShader
		{
		public:
			virtual ~VertexShader() = default;
		protected:
			VertexShader() = default;
		};

		class PixelShader
		{
		public:
			virtual ~PixelShader() = default;
		protected:
			PixelShader() = default;
		};

		class PipelineParam
		{
		public:
			virtual ~PipelineParam() = default;

			virtual void SetAsInt(const int value) = 0;
			virtual void SetAsFloat(const float value) = 0;
			virtual void SetAsMat4(const float* value) = 0;
			virtual void SetAsVec3(const float* value) = 0;
			virtual void SetAsVec2(const float* value) = 0;
		protected:
			PipelineParam() = default;
		};

		class Pipeline
		{
		public:
			virtual ~Pipeline() = default;
			virtual PipelineParam* GetParam(const char* name) = 0;
		protected:
			Pipeline() = default;
		};

		class VertexBuffer
		{
		public:
			virtual ~VertexBuffer() = default;
		protected:
			VertexBuffer() = default;
		};

		class VertexDescription
		{
		public:
			virtual ~VertexDescription() = default;
		protected:
			VertexDescription() = default;
		};

		class VertexArray
		{
		public:
			virtual ~VertexArray() = default;
		protected:
			VertexArray() = default;
		};

		// because it role, it deserved to has own class
		class IndexBuffer
		{
		public:
			virtual ~IndexBuffer() = default;
		protected:
			IndexBuffer() = default;
		};

		class Texture
		{
		public:
			virtual ~Texture() = default;
		protected:
			Texture(const TextureCreateInfo& info){m_TexInfo=info; };
		public:
			TextureCreateInfo m_TexInfo;
		};

		// Encapsulates the rasterizer state
		class RasterState
		{
		public:

			virtual ~RasterState() {}

		protected:

			RasterState() {}
		};
		// Describes a vertex element's type
		enum VertexElementType
		{
			VERTEXELEMENTTYPE_BYTE = 0,
			VERTEXELEMENTTYPE_SHORT,
			VERTEXELEMENTTYPE_INT,

			VERTEXELEMENTTYPE_UNSIGNED_BYTE,
			VERTEXELEMENTTYPE_UNSIGNED_SHORT,
			VERTEXELEMENTTYPE_UNSIGNED_INT,

			VERTEXELEMENTTYPE_BYTE_NORMALIZE,
			VERTEXELEMENTTYPE_SHORT_NORMALIZE,
			VERTEXELEMENTTYPE_INT_NORMALIZE,

			VERTEXELEMENTTYPE_UNSIGNED_BYTE_NORMALIZE,
			VERTEXELEMENTTYPE_UNSIGNED_SHORT_NORMALIZE,
			VERTEXELEMENTTYPE_UNSIGNED_INT_NORMALIZE,

			VERTEXELEMENTTYPE_HALF_FLOAT,
			VERTEXELEMENTTYPE_FLOAT,
			VERTEXELEMENTTYPE_DOUBLE
		};
		// Describes a vertex element within a vertex buffer
		struct VertexElement
		{
			unsigned int index;			// location binding to vertex shader
			VertexElementType type;		// type of vertex element
			std::size_t size;					// number of components
			std::size_t stride;					// number of bytes between each successive element (leave zero for this to be assumed to be size times size of type)
			long long offset;			// offset where first occurrence of this vertex element resides in the buffer
		};

		enum ShaderAttributeLocation
		{
			SHADER_POSITION_ATTRIBUTE,
			SHADER_NORMAL_ATTRIBUTE,
			SHADER_TEXCOORD_ATTRIBUTE,
			SHADER_COLOR_ATTRIBUTE,
			SHADER_BLEND1_ATTRIBUTE,
			SHADER_BLEND2_ATTRIBUTE,
			SHADER_BLEND3_ATTRIBUTE,
			SHADER_BLEND4_ATTRIBUTE,
			SHADER_NUM
		};

		const static char* uMODEL	= "uModel";
		const static char* uMVP	= "uMVP";
		const static char* aPOS	= "aPos";
		enum Primitive
		{
			PRIMITIVE_POINT = 0,
			PRIMITIVE_LINES,
			PRIMITIVE_LINE_STRIP,
			PRIMITIVE_LINE_LOOP,
			PRIMITIVE_LINES_ADJACENCY,
			PRIMITIVE_LINE_STRIP_ADJACENCY,
			PRIMITIVE_TRIANGLES,
			PRIMITIVE_TRIANGLE_STRIP,
			PRIMITIVE_TRIANGLE_FAN,
			PRIMITIVE_TRIANGLE_STRIP_ADJACENCY
		};
		

		enum Winding
		{
			WINDING_CW = 0,
			WINDING_CCW,
			WINDING_MAX
		};

		enum Face
		{
			FACE_FRONT = 0,
			FACE_BACK,
			FACE_FRONT_AND_BACK,
			FACE_MAX
		};

		enum RasterMode
		{
			RASTERMODE_POINT = 0,
			RASTERMODE_LINE,
			RASTERMODE_FILL,
			RASTERMODE_MAX
		};

		// Encapsulates the depth/stencil state
		class DepthStencilState
		{
		public:

			// virtual destructor to ensure subclasses have a virtual destructor
			virtual ~DepthStencilState() {}

		protected:

			// protected default constructor to ensure these are never created
			// directly
			DepthStencilState() {}
		};

		
		enum Compare
		{
			// Test comparison never passes
			COMPARE_NEVER = 0,

			// Test comparison passes if the incoming value is less than the stored value.
			COMPARE_LESS,

			// Test comparison passes if the incoming value is equal to the stored value.
			COMPARE_EQUAL,

			// Test comparison passes if the incoming value is less than or equal to the stored value.
			COMPARE_LEQUAL,

			// Test comparison passes if the incoming value is greater than the stored value.
			COMPARE_GREATER,

			// Test comparison passes if the incoming value is not equal to the stored value.
			COMPARE_NOTEQUAL,

			// Test comparison passes if the incoming value is greater than or equal to the stored value.
			COMPARE_GEQUAL,

			// Test comparison always passes.
			COMPARE_ALWAYS,

			COMPARE_MAX
		};

		enum StencilAction
		{
			// Keeps the current value.
			STENCIL_KEEP = 0,

			// Sets the stencil buffer to zero.
			STENCIL_ZERO,

			// Sets the stencil buffer to the reference value masked with the write mask.
			STENCIL_REPLACE,

			// Increments the current stencil buffer value and clamps to maximum unsigned value.
			STENCIL_INCR,

			// Increments the current stencil buffer value and wraps the stencil buffer to zero when passing the maximum representable unsigned value.
			STENCIL_INCR_WRAP,

			// Decrements the current stencil buffer value and clamps to zero.
			STENCIL_DECR,

			// Decrements the current stencil buffer value and wraps the stencil buffer value to the maximum unsigned value.
			STENCIL_DECR_WRAP,

			// Bitwise invert of the current stencil buffer value.
			STENCIL_INVERT,

			STENCIL_MAX
		};

		struct DepthStencilConfig
		{
			bool	DepthEnable = true;
			// Enable/Disable writing to depth buffer
			bool	DepthMask = true;
			Compare Depthfunc = COMPARE_LESS;

			bool			FrontStencilEnabled = false;
			Compare			FrontStencilCompare = COMPARE_NOTEQUAL;
			// stencil test fail
			StencilAction	FrontStencilFail = STENCIL_KEEP;
			// stencil test pass, depth test fail
			StencilAction	FrontDepthFail = STENCIL_KEEP;
			// both stencil/depth test pass
			StencilAction	FrontStencilPass = STENCIL_REPLACE;
			int				FrontRef = 1;
			unsigned int	FrontCompareMask = 0xFFFFFFFF;
			unsigned int	FrontWriteMask = 0xFFFFFFFF;

			/*bool			BackStencilEnabled = false;
			Compare			BackStencilCompare = COMPARE_NOTEQUAL;
			StencilAction	BackStencilFail = STENCIL_KEEP;
			StencilAction	BackStencilPass = STENCIL_REPLACE;
			StencilAction	BackDepthFail = STENCIL_KEEP;
			int				BackRef = 1;
			unsigned int	BackCompareMask = 0xFFFFFFFF;
			unsigned int	BackWriteMask = 0xFFFFFFFF;*/
		};

		
		
		class RenderDevice :public ISubSystem
		{
		public:
			virtual ~RenderDevice() {}

			virtual VertexShader*		CreateVertexShader(const char* code) = 0;
			virtual PixelShader*		CreatePixelShader(const char* code) = 0;
			virtual Pipeline*			CreatePipeline(VertexShader*, PixelShader*) = 0;
			virtual VertexBuffer*		CreateVertexBuffer(long long size, const void* data = nullptr) = 0;
			virtual VertexDescription*	CreateVertexDescription(unsigned int numElement, const VertexElement* pElement) = 0;
			virtual VertexArray*		CreateVertexArray(unsigned int numBuffer, VertexBuffer**vertexBuffer, VertexDescription** vertexDescription) = 0;
			virtual IndexBuffer*		CreateIndexBuffer(unsigned int size, const void* pData = nullptr) = 0;
			virtual Texture*			CreateTexture(const TextureCreateInfo& info, bool isCompress = false) = 0;
			virtual DepthStencilState*	CreateDepthStencilState(const DepthStencilConfig& config) = 0;

			virtual void				SetVertexArray(VertexArray*) = 0;
			virtual void				SetPipeline(Pipeline*) = 0;
			virtual void				SetIndexBuffer(IndexBuffer*) = 0;
			virtual void				SetTexture(unsigned int slot, Texture*) = 0;
			virtual void				SetDepthStencilState(DepthStencilState* state = nullptr) = 0;

			virtual void				Clear(float red = 0.0f, float green = 0.0f, float blue = 0.0f, float alpha = 1.0f, float depth = 1.0f) = 0;
			virtual void				Draw(int first, int count, int primcount = 0, Primitive primitive = PRIMITIVE_TRIANGLES) = 0;
			virtual void				DrawElement(int count, const void * indices, int primcount = 0, Primitive primitive = PRIMITIVE_TRIANGLES) = 0;

			virtual void				Render() =0;
			virtual render::ICamera*	VGetCurrentCamera() = 0;
			virtual void				VSetCurrentCamera(render::ICamera * cam) = 0;

			virtual void				AddExtraPass(RenderPass* pass) = 0;
			virtual RenderPass*			GetRenderPass(const std::string& name = "Default") = 0;

		};
	}
}
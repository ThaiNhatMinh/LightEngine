#pragma once

#include "ICamera.h"
#include "RenderPass.h"
namespace Light
{
	class IActor;
	class IScene;
	namespace render
	{
		enum BufferUsage
		{
			// The data store contents will be modified repeatedly and used many times.
			STREAM_DRAW = 0, 
			STREAM_READ, 
			STREAM_COPY, 
			// The data store contents will be modified repeatedly and used many times.
			STATIC_DRAW, 
			STATIC_READ, 
			STATIC_COPY, 
			// The data store contents will be modified repeatedly and used many times.
			DYNAMIC_DRAW, 
			DYNAMIC_READ, 
			DYNAMIC_COPY
		};
		enum BufferAccess
		{
			READ_ONLY = 0,
			WRITE_ONLY,
			READ_WRITE
		};
		enum TextureUnit
		{
			UNIT_AMBIENT = 0,
			UNIT_DIFFUSE,
			UNIT_SPECULAR,
			UNIT_SKYBOX,
			UNIT_DEPTH,

		};
		enum ColorFormat
		{
			// Base Internal Formats
			FORMAT_DEPTH_COMPONENT = 0,
			FORMAT_DEPTH_STENCIL,
			FORMAT_STENCIL_INDEX,
			FORMAT_RED,
			FORMAT_RG,
			FORMAT_RGB,
			FORMAT_RGBA,

			// Sized Internal Formats
			FORMAT_R8,
			FORMAT_R8UI,
			FORMAT_R8I,
			FORMAT_R16UI,
			FORMAT_R16I,
			FORMAT_R32UI,
			FORMAT_R32I,
			FORMAT_RG8,
			FORMAT_RG8UI,
			FORMAT_RG8I,
			FORMAT_RG16UI,
			FORMAT_RG16I,
			FORMAT_RG32UI,
			FORMAT_RG32I,
			FORMAT_RGB8,
			FORMAT_RGB565,
			FORMAT_RGBA8,
			FORMAT_SRGB8_ALPHA8,
			FORMAT_RGB5_A1,
			FORMAT_RGBA4,
			FORMAT_RGB10_A2,
			FORMAT_RGBA8UI,
			FORMAT_RGBA8I,
			FORMAT_RGB10_A2UI,
			FORMAT_RGBA16UI,
			FORMAT_RGBA16I,
			FORMAT_RGBA32I,
			FORMAT_RGBA32UI,

			// Sized Depth and Stencil Internal Formats
			FORMAT_DEPTH_COMPONENT16,
			FORMAT_DEPTH_COMPONENT24,
			FORMAT_DEPTH_COMPONENT32F,
			FORMAT_DEPTH24_STENCIL8,
			FORMAT_DEPTH32F_STENCIL8,
			FORMAT_STENCIL_INDEX8,
			// S3TC formats
			FORMAT_COMPRESSED_RGBA_S3TC_DXT1_EXT,
			FORMAT_COMPRESSED_RGBA_S3TC_DXT3_EXT,
			FORMAT_COMPRESSED_RGBA_S3TC_DXT5_EXT,
			FORMAT_MAX
		};

		enum Attachment
		{
			COLOR_ATTACHMENT = 0,
			DEPTH_ATTACHMENT,
			STENCIL_ATTACHMENT,
			DEPTH_STENCIL_ATTACHMENT,
			ATTACHMENT_MAX
		};

		enum TextureType
		{
			TEXTURE_2D = 0, 
			PROXY_TEXTURE_2D, 
			TEXTURE_1D_ARRAY, 
			PROXY_TEXTURE_1D_ARRAY, 
			TEXTURE_RECTANGLE, 
			PROXY_TEXTURE_RECTANGLE, 
			TEXTURE_CUBE_MAP,
			TEXTURE_CUBE_MAP_POSITIVE_X, 
			TEXTURE_CUBE_MAP_NEGATIVE_X, 
			TEXTURE_CUBE_MAP_POSITIVE_Y, 
			TEXTURE_CUBE_MAP_NEGATIVE_Y, 
			TEXTURE_CUBE_MAP_POSITIVE_Z, 
			TEXTURE_CUBE_MAP_NEGATIVE_Z, 
			PROXY_TEXTURE_CUBE_MAP,
			TEXTURE_MAX
		};
		enum NumberFormat
		{
			UNSIGNED_BYTE = 0, 
			BYTE, 
			UNSIGNED_SHORT, 
			SHORT, 
			UNSIGNED_INT, 
			INT, 
			HALF_FLOAT, 
			FLOAT, 
			UNSIGNED_SHORT_5_6_5, 
			UNSIGNED_SHORT_4_4_4_4, 
			UNSIGNED_SHORT_5_5_5_1, 
			UNSIGNED_INT_2_10_10_10_REV, 
			UNSIGNED_INT_10F_11F_11F_REV, 
			UNSIGNED_INT_5_9_9_9_REV, 
			UNSIGNED_INT_24_8, 
			FLOAT_32_UNSIGNED_INT_24_8_REV,
			NUMBERFORMAT_MAX
		};
		struct TextureCreateInfo
		{
			TextureType eTarget;			//  GL_TEXTURE_2D, GL_PROXY_TEXTURE_2D, GL_TEXTURE_1D_ARRAY, GL_PROXY_TEXTURE_1D_ARRAY, GL_TEXTURE_RECTANGLE, GL_PROXY_TEXTURE_RECTANGLE
			int iLevel;						// Specifies the level-of-detail number.
			ColorFormat iInternalFormat;			// Specifies the number of color components in the texture.
			unsigned int uiWidth, uiHeight;	// Specifies the width/height of the texture image
			ColorFormat eFormat;			// Specifies the format of the pixel data.
			NumberFormat eType;				// Specifies the data type of the pixel data
			void* pData;					// Specifies a pointer to the image data in memory. 
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

			virtual void SetAsFloatV(const float* value,int num) = 0;
			virtual void SetAsMat4V(const float* value, int num) = 0;
			virtual void SetAsVec3V(const float* value, int num) = 0;
			virtual void SetAsVec2V(const float* value, int num) = 0;
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
			virtual void SetData(long long size, const void* data, BufferUsage usage = STATIC_DRAW) = 0;
			virtual void* Map(BufferAccess access) = 0;
			virtual bool UnMap() = 0;
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
		const static char* uMVP		= "uMVP";
		const static char* uTex		= "uTex";
		const static char* uCubeTex = "uCubeTex";
		const static char* uCameraPos = "uCameraPos";
		const static char* uColor	= "uColor";
		const static char* uSkeTransform = "uTransform[0]";
		const static char* aPOS		= "aPos";

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

		struct CullFaceConfig
		{
			bool CullEnable = true;
			Winding FrontFaceWinding = WINDING_CCW;
			Face CullFace = FACE_BACK;
			RasterMode FillMode = RASTERMODE_FILL;
		};
		
		enum BlendFactor
		{
			FACTOR_ZERO = 0, 
			FACTOR_ONE, 
			FACTOR_SRC_COLOR, 
			FACTOR_ONE_MINUS_SRC_COLOR, 
			FACTOR_DST_COLOR, 
			FACTOR_ONE_MINUS_DST_COLOR, 
			FACTOR_SRC_ALPHA, 
			FACTOR_ONE_MINUS_SRC_ALPHA, 
			FACTOR_DST_ALPHA, 
			FACTOR_ONE_MINUS_DST_ALPHA,
			FACTOR_CONSTANT_COLOR, 
			FACTOR_ONE_MINUS_CONSTANT_COLOR, 
			FACTOR_CONSTANT_ALPHA,  
			FACTOR_ONE_MINUS_CONSTANT_ALPHA,
			FACTOR_MAX
		};

		enum BlendFunc
		{
			FUNC_ADD, 
			FUNC_SUBTRACT, 
			FUNC_REVERSE_SUBTRACT, 
			MIN, 
			MAX
		};

		struct BlendConfig
		{
			bool Enable = false;
			BlendFactor sfactor = FACTOR_SRC_ALPHA;
			BlendFactor dfactor = FACTOR_ONE_MINUS_SRC_ALPHA;
			BlendFunc func = FUNC_ADD;
		};

		class BlendingState
		{
		public:
			virtual ~BlendingState() = default;
		};

		class RenderBuffer
		{
		public:
			virtual ~RenderBuffer() = default;
		};
		class FrameBuffer
		{
		public:
			virtual ~FrameBuffer() = default;

			virtual void AttachTexture(Attachment attachment, Texture* pTex, int level) = 0;
			virtual void AttachRenderBuffer(Attachment attachment, RenderBuffer* pBuffer) = 0;
			virtual void Begin() = 0;
			virtual void End() = 0;
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
			virtual RasterState*		CreateRasterState(const CullFaceConfig& config) = 0;
			virtual BlendingState*		CreateBlendingState(const BlendConfig& config) = 0;
			virtual FrameBuffer*		CreateFrameBuffer() = 0;
			virtual RenderBuffer*		CreateRenderBuffer(ColorFormat format, int w = -1, int h = -1) = 0;

			virtual void				SetVertexArray(VertexArray*) = 0;
			virtual void				SetPipeline(Pipeline*) = 0;
			virtual void				SetIndexBuffer(IndexBuffer*) = 0;
			virtual void				SetTexture(TextureUnit slot, Texture*) = 0;
			virtual void				SetDepthStencilState(DepthStencilState* state = nullptr) = 0;
			virtual void				SetRasterState(RasterState* state = nullptr) = 0;
			virtual void				SetBlendingState(BlendingState* state = nullptr) = 0;

			virtual void				Clear(float red = 0.2f, float green = 0.2f, float blue = 0.2f, float alpha = 1.0f, float depth = 1.0f) = 0;
			virtual void				Draw(int first, int count, int primcount = 0, Primitive primitive = PRIMITIVE_TRIANGLES) = 0;
			virtual void				DrawElement(int count, const void * indices, int primcount = 0, Primitive primitive = PRIMITIVE_TRIANGLES) = 0;

			virtual void				Render() =0;
			virtual render::ICamera*	VGetCurrentCamera() = 0;
			virtual void				VSetCurrentCamera(render::ICamera * cam) = 0;

			virtual void				AddExtraPass(RenderPass* pass) = 0;
			virtual RenderPass*			GetRenderPass(const std::string& name = "Default") = 0;

			virtual Texture*			GetSkyBoxTexture() = 0;
			virtual IScene*				GetScene() = 0;

		};
	}
}
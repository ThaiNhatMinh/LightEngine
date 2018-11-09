#pragma once
#include "..\Interface\IVGUI.h"
#include "..\Interface\IWindow.h"
#include "..\Interface\IRenderSystem.h"
#include "FreeTypeFont.h"
namespace Light
{
	class VGUI : public vgui::IVGUI
	{
	public:
		VGUI(IContext* c);
		~VGUI();



		void		Render();
		void		Update(float dt);
		
		virtual void		VDrawText(const std::string& str, const  glm::vec2&position, vgui::Font* pFont = nullptr);
		virtual void		VDraw(render::Texture* pTex, const glm::vec2&position, const glm::vec2& scale = glm::vec2(1,1));
		virtual vgui::Font*	VCreateFont(std::string filename);
		virtual const char* VGetName();
	private:
		void RenderText();
		void RenderTexture();
	private:
		std::vector<std::unique_ptr<vgui::Font>>		m_FontLists;
		glm::mat4						m_Proj;
		IWindow* m_pWindow;
		render::IRenderSystem* m_pRenderS;
		render::RenderDevice* m_pRenderD;
		render::PipelineParam* m_uMVP;
		render::PipelineParam* m_uPos;
		render::PipelineParam* m_uSize;
		render::PipelineParam* m_uIsText;
		std::unique_ptr<render::VertexArray> m_pVAO;
		std::unique_ptr < render::VertexBuffer> m_pVBO;
		std::unique_ptr < render::Pipeline> m_UIShader;
		vgui::FTFont* m_DefaultFont;

	private:
		struct TextRender
		{
			glm::vec2 pos;
			std::string text;
			vgui::FTFont* font;
		};

		struct TextureRender
		{
			render::Texture* tex;
			glm::vec2 pos;
			glm::vec2 scale;
		};
		// render text
		std::list<TextRender> m_Texts;
		std::list<TextureRender> m_Textures;
	};


}
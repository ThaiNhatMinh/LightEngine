#include <pch.h>
#include "VGUI.h"
#include "FreeTypeFont.h"
#include "..\Interface\IResourceManager.h"
#include "..\Interface\IRenderSystem.h"
namespace Light
{
	VGUI::VGUI(IContext* c):m_DefaultFont(nullptr)
	{
		
		auto pResourceM = c->GetSystem<resources::IResourceManager>();
		m_pRenderS = c->GetSystem<render::IRenderSystem>();
		m_pRenderD = m_pRenderS->GetRenderDevice();
		
		m_pWindow = c->GetSystem<IWindow>();

		int w, h;
		m_pWindow->VGetWindowSize(w,h);
		m_Proj = glm::ortho(0.0f, (float)w, 0.0f, (float)h);

		auto VS = m_pRenderD->CreateVertexShader(pResourceM->VGetShaderCode("UI.vs")->Get());
		auto PS = m_pRenderD->CreatePixelShader(pResourceM->VGetShaderCode("UI.fs")->Get());
		m_UIShader.reset(m_pRenderD->CreatePipeline(VS, PS));
		m_uMVP = m_UIShader->GetParam("uMVP");
		m_uPos = m_UIShader->GetParam("uPos");
		m_uSize = m_UIShader->GetParam("uSize");
		m_uIsText = m_UIShader->GetParam("isText");
		m_uColor = m_UIShader->GetParam("objColor");
		glm::vec2 buffer[4] = {glm::vec2(0,0), glm::vec2(0,1), glm::vec2(1,0),glm::vec2(1,1)};

		m_pVBO.reset(m_pRenderD->CreateVertexBuffer(sizeof(glm::vec2) * 4, buffer));
		render::VertexElement element;
		element.index = render::SHADER_POSITION_ATTRIBUTE;
		element.offset = 0;
		element.num = 2;
		element.stride = sizeof(glm::vec2);
		element.type = render::VertexElementType::VERTEXELEMENTTYPE_FLOAT;
		auto pTemp = m_pRenderD->CreateVertexDescription(1, &element);
		auto pTemp1 = m_pVBO.get();
		m_pVAO.reset(m_pRenderD->CreateVertexArray(1, &pTemp1, &pTemp));


		vgui::FTFont::InitFreeTypeFont();


		m_DefaultFont = (vgui::FTFont*)this->VCreateFont("GameAssets\\FONTS\\segoeui.ttf");
		c->VAddSystem(this);
		
	}

	VGUI::~VGUI()
	{
		vgui::FTFont::ReleaseFreeTypeFont();
	}

	void VGUI::Render()
	{
		//glDisable(GL_CULL_FACE);
		//glEnable(GL_BLEND);
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		m_pRenderD->SetCullFace(false);
		m_pRenderD->SetBlend(true);
		m_pRenderD->SetPipeline(m_UIShader.get());
		m_uMVP->SetAsMat4(glm::value_ptr(m_Proj));
		m_pRenderD->SetVertexArray(m_pVAO.get());
		//RenderTexture();
		//RenderText();
		while (m_List.size())
		{
			auto& el = m_List.front();
			m_List.pop_front();
			if (el.type == Text) RenderText(el._text);
			else RenderTexture(el._tex);
		}
		
	}

	void VGUI::Update(float dt)
	{

		//m_Root->Update(dt, m_pWindows->GetMousePos());
	}
	void VGUI::VDrawText(const std::string& str, const  glm::vec2& position, vgui::Font* pFont)
	{
		TextRender tr;
		tr.pos = position;
		tr.text = str;
		if(pFont) tr.font = (vgui::FTFont*)pFont;
		else tr.font = m_DefaultFont;

		UIRender ur;
		ur.type = Text;
		ur._text = tr;
		m_List.push_back(ur);
	}
	void VGUI::VDraw(render::Texture * pTex,const glm::vec2& position,const glm::vec4&color, const glm::vec2&scale)
	{
		TextureRender tr;
		tr.pos = position;
		tr.tex = pTex;
		tr.scale = scale;
		tr.color = color;

		UIRender ur;
		ur.type = Texture;
		ur._tex = tr;

		m_List.push_back(ur);
	}
	vgui::Font * VGUI::VCreateFont(std::string filename)
	{
		std::string fontName = filename.substr(filename.find_last_of('/') + 1);
		for (auto& el : m_FontLists)
			if (el->GetName() == fontName) return false;

		m_FontLists.push_back(std::unique_ptr<vgui::FTFont>(DEBUG_NEW vgui::FTFont(m_pRenderS,fontName, filename,22)));

		return m_FontLists.back().get();
	}
	const char * VGUI::VGetName()
	{
		static const char* pName = typeid(vgui::IVGUI).name();
		return pName;
	}
	void VGUI::RenderText(TextRender& el)
	{
		m_uIsText->SetAsInt(1);

		float x = 0;// m_Pos.x;
		float y = 0;// m_Pos.y;
		if (!el.font) return;
		for (std::size_t i = 0; i < el.text.size(); i++)
		{
			vgui::FTFont::FontChar* ch = el.font->GetChar(el.text[i]);
			glm::vec2 pos(x + ch->Bearing[0], y - (ch->size[1] - ch->Bearing[1]));
			pos += el.pos;
			glm::vec2 size(ch->size[0], ch->size[1]);

			m_uPos->SetAsVec2(glm::value_ptr(pos));
			m_uSize->SetAsVec2(glm::value_ptr(size));

			m_pRenderD->SetTexture(render::TextureUnit::UNIT_AMBIENT, ch->iTextureID);
			m_pRenderD->Draw(0, 4, 0, render::PRIMITIVE_TRIANGLE_STRIP);

			x += (ch->advance >> 6); // Bitshift by 6 to get value in pixels (2^6 = 64)
		}
		
	}
	void VGUI::RenderTexture(TextureRender& el)
	{
		
		m_uIsText->SetAsInt(0);

		if (!el.tex) return;

		glm::vec2 size(el.tex->GetWidth()*el.scale.x, el.tex->GetHeight()*el.scale.y);
		m_uPos->SetAsVec2(glm::value_ptr(el.pos));
		m_uSize->SetAsVec2(glm::value_ptr(size));
		m_uColor->SetAsVec4(glm::value_ptr(el.color));
		m_pRenderD->SetTexture(render::TextureUnit::UNIT_AMBIENT, el.tex);
		m_pRenderD->Draw(0, 4, 0, render::PRIMITIVE_TRIANGLE_STRIP);

	}

}
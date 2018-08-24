#include <pch.h>
#include "VGUI.h"


class VGUI::UIFactoryInterface
{
public:
	~UIFactoryInterface() {};
	virtual UIElement* Create() = 0;
};

template<class T>
class VGUI::UIFactory : public VGUI::UIFactoryInterface
{
public:
	UIFactory() = default;
	~UIFactory() = default;
	virtual UIElement* Create();

};

template<class T>
UIElement * VGUI::UIFactory<T>::Create()
{
	return new T();
}

VGUI::VGUI(Context* c):m_Root(new UIGroup())
{
	m_ControlFactory.resize(CTRL_COUNT);
	m_ControlFactory[CTRL_TEXT] = std::unique_ptr<UIFactory<UIText>>(new UIFactory<UIText>());
	m_ControlFactory[CTRL_IMAGE] = std::unique_ptr<UIFactory<UIImage>>(new UIFactory<UIImage>());

	m_pWindows = c->GetSystem<Windows>();
	vec2 size = m_pWindows->GetWindowSize();
	m_Proj = glm::ortho(0.0f, size.x, 0.0f, size.y);

	m_UIShader = c->GetSystem<Resources>()->GetShader("UI");

	FTFont::InitFreeTypeFont();
	AddFont("Default", "GameAssets\\FONTS\\segoeui.ttf");
	c->AddSystem(this);

}

VGUI::~VGUI()
{
	FTFont::ReleaseFreeTypeFont();
}

void VGUI::Render()
{
	glDisable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	m_UIShader->Use();
	m_UIShader->SetUniformMatrix("MVP", glm::value_ptr(m_Proj));
	m_Root->Render();
	glDisable(GL_BLEND);
	glEnable(GL_CULL_FACE);
}

void VGUI::Update(float dt)
{
	
	m_Root->Update(dt, m_pWindows->GetMousePos());
}

UIGroup * VGUI::GetRoot()
{
	return m_Root.get();
}

bool VGUI::AddFont(const string & fontname, const string & fontfile)
{
	for (auto& el : m_FontLists)
		if (el->GetName() == fontname) return false;

	m_FontLists.push_back(std::unique_ptr<FTFont>(new FTFont(fontname, fontfile)));

	return true;
}

Shader * VGUI::GetShader()
{
	return m_UIShader;
}

const mat4 & VGUI::GetProj()
{
	return m_Proj;
}

FTFont * VGUI::GetFont(const string & fontname)
{
	for (auto& el : m_FontLists)
		if (el->GetName() == fontname) return el.get();

	return nullptr;
}

UIElement * VGUI::CreateElement(Control ctrl)
{
	if (ctrl >= CTRL_COUNT || ctrl<0) return nullptr;
	
	auto ptr = m_ControlFactory[ctrl]->Create();

	ptr->OnInit(this);

	return ptr;
}


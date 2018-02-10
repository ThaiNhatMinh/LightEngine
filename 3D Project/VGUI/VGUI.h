#pragma once

#include "UIMesh.h"
#include "UIElement.h"
#include "UIImage.h"
#include "UIText.h"

class VGUI
{
public:
	VGUI();
	~VGUI();

	void		Render();
	void		Update(float dt);
	UIElement*	GetRoot();
	bool		AddFont(const string& fontfile);

private:
	std::unique_ptr<UIElement> m_Root;
};
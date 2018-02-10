#pragma once


class UIText : public UIElement
{
public:
	UIText();
	~UIText();

	virtual void Render()override;


	const string&	GetText()const {
		return m_Text;
	};
	void			SetText(const string& text) {
		m_Text = text;
	};

private:
	string m_Text;
};
#include "pch.h"

UIText::UIText(VGUI * pVGUI)
{
	m_Mesh.Init();
	m_UIShader = pVGUI->GetShader();
	m_Font = pVGUI->GetFont("Default");
}

void UIText::Render()
{
	float x = m_Pos.x;
	float y = m_Pos.y;

	

	glBindVertexArray(m_Mesh.VAO);

	std::string::const_iterator c;
	for (c = m_Text.begin(); c != m_Text.end(); c++)
	{
		FTFont::FontChar* ch = m_Font->GetChar(*c);
		float xpos = x + ch->Bearing[0];
		float ypos = y - (ch->size[1] - ch->Bearing[1]);
		GLfloat w = ch->size[0];
		GLfloat h = ch->size[1];
		// Update VBO for each character
		GLfloat vertices[6][4] = {
			{ xpos,     ypos + h,   0.0, 0.0 },
			{ xpos,     ypos,       0.0, 1.0 },
			{ xpos + w, ypos,       1.0, 1.0 },

			{ xpos,     ypos + h,   0.0, 0.0 },
			{ xpos + w, ypos,       1.0, 1.0 },
			{ xpos + w, ypos + h,   1.0, 0.0 }
		};

		// Render glyph texture over quad

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, ch->iTextureID);
		m_UIShader->SetUniform("tex", 0);
		// Update content of VBO memory
		glBindBuffer(GL_ARRAY_BUFFER, m_Mesh.VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		// Render quad
		glDrawArrays(GL_TRIANGLES, 0, 6);
		// Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
		//cout << xpos <<' ' << ypos<< endl;
		x += (ch->advance >> 6); // Bitshift by 6 to get value in pixels (2^6 = 64)
	}
}

const string & UIText::GetText() const {
	return m_Text;
}

 void UIText::SetText(const string & text) {
	m_Text = text;
}

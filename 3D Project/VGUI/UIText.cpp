#include "pch.h"

UIText::UIText()
{

}

void UIText::OnInit(VGUI *pVGUI)
{
	m_UIShader = pVGUI->GetShader();
	m_Font = pVGUI->GetFont("Default");
}

void UIText::Render()
{
	float x = m_Pos.x;
	float y = m_Pos.y;

	if (m_Text.size() == 0) return;

	

	
	for (size_t i = 0; i<m_Text.size(); i++)
	{
		glBindVertexArray(m_Meshs[i].Mesh.VAO);
		// Render glyph texture over quad

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_Meshs[i].texID);
		m_UIShader->SetUniform("tex", 0);
		m_UIShader->SetUniform("objColor", vec3(1.0f));
		
		// Render quad
		glDrawArrays(GL_TRIANGLES, 0, 6);
		
	}
}

const string & UIText::GetText() const {
	return m_Text;
	
}

 void UIText::SetText(const string & text) {
	m_Text = text;
	UpdateInternalData();
}

void UIText::UpdateInternalData()
 {
	 if (m_Text.size() == 0) return;
	 float x = m_Pos.x;
	 float y = m_Pos.y;

	 m_Meshs.resize(m_Text.size());

	 for (size_t i = 0; i<m_Text.size(); i++)
	 {
		 m_Meshs[i].Mesh.Init();
		 FTFont::FontChar* ch = m_Font->GetChar(m_Text[i]);
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


		 m_Meshs[i].texID = ch->iTextureID;
		 // Update content of VBO memory
		 glBindBuffer(GL_ARRAY_BUFFER, m_Meshs[i].Mesh.VBO);
		 glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		 glBindBuffer(GL_ARRAY_BUFFER, 0);

		 x += (ch->advance >> 6); // Bitshift by 6 to get value in pixels (2^6 = 64)
	 }
 }

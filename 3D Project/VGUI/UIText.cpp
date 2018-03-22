#include <pch.h>

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
		m_Meshs[i].Mesh.VAO.Bind();
		// Render glyph texture over quad

		//glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_Meshs[i].texID);
		m_UIShader->SetUniform("tex", 0);
		m_UIShader->SetUniform("isText", 1);
		m_UIShader->SetUniform("objColor", vec3(1.0f));
		m_UIShader->SetUniform("Translate", m_Pos + m_Meshs[i].Pos);
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
	 float x = 0;// m_Pos.x;
	 float y = 0;// m_Pos.y;

	 m_Meshs.clear();
	 //m_Meshs.resize(m_Text.size());

	 for (std::size_t i = 0; i<m_Text.size(); i++)
	 {
		 TextRenderInfo tr;
		 FTFont::FontChar* ch = m_Font->GetChar(m_Text[i]);
		 float xpos = x + ch->Bearing[0];
		 float ypos = y - (ch->size[1] - ch->Bearing[1]);
		 GLfloat w = ch->size[0];
		 GLfloat h = ch->size[1];
		 // Update VBO for each character
		 GLfloat vertices[6][4] = {
			 { 0,     h,   0.0, 0.0 },
		 { 0,     0,       0.0, 1.0 },
		 {  w, 0,       1.0, 1.0 },

		 { 0,     h,   0.0, 0.0 },
		 {  w, 0,       1.0, 1.0 },
		 {  w,  h,   1.0, 0.0 }
		 };

		 // Render glyph texture over quad


		 tr.texID = ch->iTextureID;
		 // Update content of VBO memory
		 tr.Mesh.VBO.Bind();
		 tr.Mesh.VBO.SetData(sizeof(vertices), vertices, GL_STATIC_DRAW);
		 tr.Mesh.VBO.UnBind();

		 tr.Pos = vec2(xpos, ypos);

		 x += (ch->advance >> 6); // Bitshift by 6 to get value in pixels (2^6 = 64)

		 m_Meshs.push_back(std::move(tr));
	 }
 }

UIText::TextRenderInfo::TextRenderInfo(TextRenderInfo && other):Mesh(std::move(other.Mesh)),texID(other.texID),Pos(other.Pos)
{
	
}

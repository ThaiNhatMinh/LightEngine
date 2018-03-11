#include "pch.h"

UIImage::UIImage():m_Texture(nullptr)
{
}

UIImage::~UIImage()
{
}

void UIImage::OnInit(VGUI *pVGUI)
{
	m_UIShader = pVGUI->GetShader();
}


void UIImage::Render()
{
	
	if (m_Texture==nullptr) return;
	m_Mesh.VAO.Bind();
	// Render glyph texture over quad

	//glActiveTexture(GL_TEXTURE0);
	m_Texture->Bind();
	m_UIShader->SetUniform("tex", 0);
	m_UIShader->SetUniform("isText", 0);
	m_UIShader->SetUniform("objColor", vec3(1.0f));
	m_UIShader->SetUniform("Translate", m_Pos);
	// Render quad
	glDrawArrays(GL_TRIANGLES, 0, 6);

}

Texture * UIImage::GetImage() {
	return m_Texture;
}

void UIImage::SetImage(Texture * image) {
	m_Texture = image;
	UpdateInternalData();
}

void UIImage::UpdateInternalData()
{
	m_Mesh.VBO.Bind();

	float xpos = 0;
	float ypos = 0;
	float w = m_Texture->GetWidth();
	float h = m_Texture->GetHeight();

	GLfloat vertices[6][4] = {
		{ xpos,     ypos + h,   0.0, 0.0 },
	{ xpos,     ypos,       0.0, 1.0 },
	{ xpos + w, ypos,       1.0, 1.0 },

	{ xpos,     ypos + h,   0.0, 0.0 },
	{ xpos + w, ypos,       1.0, 1.0 },
	{ xpos + w, ypos + h,   1.0, 0.0 }
	};

	m_Mesh.VBO.SetData(sizeof(vertices), vertices, GL_STATIC_DRAW);
	m_Mesh.VBO.UnBind();
}

#include <pch.h>
#include "RenderTarget.h"

RenderTarget::RenderTarget(GLuint W, GLuint H):VAO(),VBO(GL_ARRAY_BUFFER), m_framebuffer(), m_renderbuffer(W, H)
{	
	m_framebuffer.Bind();


	Texture::TextureCreateInfo texinfo;
	texinfo.uiHeight = H;
	texinfo.uiWidth = W;
	m_RenderTexture = new Texture(texinfo);
	m_framebuffer.AttachTexture(GL_COLOR_ATTACHMENT0, m_RenderTexture);
	
	m_renderbuffer.Bind();
	m_framebuffer.AttachDepthStencil(GL_DEPTH_STENCIL_ATTACHMENT,m_renderbuffer);
	
	
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	float quadVertices[] = {
		-1.0f,  1.0f, 
		-1.0f, -1.0f, 
		1.0f,  1.0f,
		1.0f, -1.0f
	};
	// create mesh
	VAO.Bind();
	VBO.Bind();
	VBO.SetData(sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
	VAO.SetAttibutePointer(SHADER_POSITION_ATTRIBUTE, 2, GL_FLOAT, sizeof(vec2), 0);
	
}

RenderTarget::~RenderTarget()
{
}

void RenderTarget::Bind()
{
	m_framebuffer.Bind();
	//glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
}

void RenderTarget::UnBind()
{
	m_framebuffer.UnBind();
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderTarget::Render()
{
	VAO.Bind();
	m_RenderTexture->Bind();
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

#include "pch.h"

SkyBox::SkyBox(ActorId id):Actor(id),VBO(GL_ARRAY_BUFFER),EBO(GL_ELEMENT_ARRAY_BUFFER)
{
	

}

bool SkyBox::Init(const tinyxml2::XMLElement * pData)
{
	auto pNode = pData->FirstChildElement("Texture");
	if (pNode == nullptr) return false;

	const char* pFile = pNode->Attribute("File");
	if (pFile == nullptr) return false;

	m_CubeTexture = m_Context->GetSystem<Resources>()->GetTexture(pFile);
	if (m_CubeTexture == nullptr) return false;

	

	m_pShader = m_Context->GetSystem<Resources>()->GetShader("SkyBox");
	if (m_pShader == nullptr)
	{
		E_ERROR("Can't get shader SkyBox");
		return false;
	}
	std::vector<glm::vec3> vertex =
	{
		vec3(1.0,1.0,1.0),
		vec3(1.0,1.0,-1.0),
		vec3(-1.0,1.0,1.0),
		vec3(-1.0,1.0,-1.0),

		vec3(1.0,-1.0,1.0),
		vec3(1.0,-1.0,-1.0),
		vec3(-1.0,-1.0,1.0),
		vec3(-1.0,-1.0,-1.0),
	};
	std::vector<unsigned int> indices = { 0,1,2,2,1,3,
		4,5,6,6,5,7,
		0,4,5,0,5,1,
		1,5,7,1,7,3,
		3,7,6,3,6,2,
		2,6,4,2,4,0 };

	VAO.Bind();
	EBO.Bind();
	VBO.Bind();
	VBO.SetData(sizeof(vec3) * vertex.size(), &vertex[0], GL_STATIC_DRAW);

	EBO.SetData(sizeof(unsigned int)*indices.size(), &indices[0], GL_STATIC_DRAW);

	GLuint loc = m_pShader->GetAttribLocation("pos");
	VAO.SetAttibutePointer(loc, 3, GL_FLOAT, sizeof(float) * 3, 0);
	return true;
}

HRESULT SkyBox::VRender(Scene * pScene)
{
	
	ICamera* pCam = pScene->GetCurrentCamera();

	mat4 view = pCam->GetViewMatrix();
	view = mat4(glm::mat3(view));
	mat4 proj = pCam->GetProjMatrix();
	mat4 VP = proj * view;
	glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_FALSE);
	glDisable(GL_CULL_FACE);
	m_pShader->Use();
	m_pShader->SetUniformMatrix("MVP", glm::value_ptr(VP));
	m_CubeTexture->Bind();
	VAO.Bind();
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	
	glDepthMask(GL_TRUE);
	glEnable(GL_CULL_FACE);
	glDepthFunc(GL_LESS);
	return S_OK;
}

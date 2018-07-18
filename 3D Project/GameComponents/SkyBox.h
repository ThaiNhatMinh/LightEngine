#pragma once


class SkyBox : public Actor
{
public:

	SkyBox(ActorId id);
	~SkyBox() {};
	virtual bool		Init(const tinyxml2::XMLElement* pData);
	virtual HRESULT		VRender(Scene* pScene) override;
private:
	VertexArray VAO;
	BufferObject VBO, EBO;
	// skybox VAO
	//unsigned int skyboxVBO;
	Shader* m_pShader;
	Texture* m_CubeTexture;
};
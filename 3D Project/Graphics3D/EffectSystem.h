#pragma once

class EffectSystem: public ISubSystem
{
private:
	GLuint VAO, VBO;
	Mesh m_QuadMesh;
	std::vector<Sprite> m_SpriteLists;
	Shader* m_pShader;
public:
	EffectSystem() = default;
	~EffectSystem() = default;
	virtual void Init(Context* c);
	virtual void ShutDown();;
	void Render(Scene* pScene);

	void AddSprite(Sprite a) {
		m_SpriteLists.push_back(a);
	};
};
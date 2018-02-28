#pragma once



class EffectSystem: public ISubSystem
{
private:
	GLuint VAO, VBO;
	Mesh m_QuadMesh;
	std::vector<Sprite> m_SpriteLists;

	std::list<SpriteAnim*> m_List2;
	Shader* m_pShader;

	OpenGLRenderer *m_pRenderer;

private:
	void CreateSpriteEvent(std::shared_ptr<IEvent> pEvent);
public:
	EffectSystem() = default;
	~EffectSystem() = default;
	virtual void	Init(Context* c);
	virtual void	ShutDown();

	void			Update(Scene* pScene,float dt);
	void			Render(Scene* pScene);

	void			AddSprite(Sprite a);
	void			AddSprite(SpriteAnim* a);

	// Temp sprite is sprite has short life in game loop
	// It don't need to sort in z-order
	void			AddTempSprite(SpriteAnim* a);
};
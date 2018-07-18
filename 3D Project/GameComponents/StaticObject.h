#pragma once


// player character

class StaticObject : public Actor
{
public:
	StaticObject(ActorId id);
	virtual void	PostInit(void);
	~StaticObject() {};
	virtual HRESULT VRender(Scene* pScene) override;

private:
	std::unique_ptr<MeshRenderComponent> m_MeshRender;
};
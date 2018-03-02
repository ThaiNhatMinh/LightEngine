#pragma once


class Zombie :public Creature
{
public:
	Zombie(ActorId id);
	~Zombie();

	//virtual bool				Init(const tinyxml2::XMLElement* pData);
	virtual void				PostInit(void);
	virtual bool				VIsVisible(Scene * pScene) const;
	virtual HRESULT				VRender(Scene* pScene) override;
	//virtual bool				VAddChild(std::unique_ptr<Actor> kid);
	//virtual HRESULT				VRenderChildren(Scene * pScene);
	//virtual HRESULT				VOnUpdate(Scene *, float elapsedMs);

	virtual void Death();
};
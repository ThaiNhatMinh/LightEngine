#pragma once


class Player : public Actor
{
public:
	Player(ActorId id);
	virtual bool	Init(const tinyxml2::XMLElement* pData);
	virtual void	PostInit(void);
	virtual HRESULT VRender(Scene* pScene) override;
};
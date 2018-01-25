#pragma once


class PlayerView : public Actor
{
public:
	PlayerView(ActorId id);
	virtual bool	Init(const tinyxml2::XMLElement* pData);
	virtual void	PostInit(void);
	virtual HRESULT VRender(Scene* pScene) override;
	virtual HRESULT VOnUpdate(Scene *, float elapsedMs)override;
	virtual mat4	VGetGlobalTransform()override;

private:
	
	mat4	m_PVTransform;
	float	m_fFOV;
};
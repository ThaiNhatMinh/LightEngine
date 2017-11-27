#pragma once

class TerrainWorld : public IGameWorld, public Actor
{
public:
	virtual bool Init(const tinyxml2::XMLElement* pData);
	virtual void PostInit(void);
	virtual void Destroy(void);
	virtual HRESULT VRender(Scene* pScene)override;
	TerrainWorld(ActorId id);
	~TerrainWorld();
private:
	TerrainRenderComponent* TRC;
};
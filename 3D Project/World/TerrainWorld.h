#pragma once

class TerrainWorld : public IGameWorld, public Actor
{
public:
	virtual bool Init(tinyxml2::XMLElement* pData);
	virtual void PostInit(void);
	virtual void Destroy(void);
	TerrainWorld(ActorId id);
	~TerrainWorld();

};
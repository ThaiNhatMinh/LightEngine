#include "pch.h"
#include "TerrainWorld.h"




bool TerrainWorld::Init(const tinyxml2::XMLElement* pData)
{
	return Actor::Init(pData);

	
	
	
}

void TerrainWorld::PostInit(void)
{
	Actor::PostInit();
	TRC = GetComponent<TerrainRenderComponent>("TerrainRenderComponent");
}


void TerrainWorld::Destroy(void)
{
}

HRESULT TerrainWorld::VRender(Scene * pScene)
{
	
	if (TRC) TRC->Render(pScene);
	return S_OK;
}

TerrainWorld::~TerrainWorld()
{
}

TerrainWorld::TerrainWorld(ActorId id):Actor(id)
{
}


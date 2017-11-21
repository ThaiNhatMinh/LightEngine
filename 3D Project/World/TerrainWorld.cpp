#include "pch.h"
#include "TerrainWorld.h"




bool TerrainWorld::Init(const tinyxml2::XMLElement* pData)
{
	return Actor::Init(pData);

	
	
	
}

void TerrainWorld::PostInit(void)
{
	Actor::PostInit();

}


void TerrainWorld::Destroy(void)
{
}

TerrainWorld::~TerrainWorld()
{
}

TerrainWorld::TerrainWorld(ActorId id):Actor(id)
{
}


#include "..\stdafx.h"
#include "Player.h"

Player::Player(ActorId id):Actor(id)
{
}

bool Player::Init(const tinyxml2::XMLElement * pData)
{
	return Actor::Init(pData);
}

void Player::PostInit(void)
{
	Actor::PostInit();
}

HRESULT Player::VRender(Scene * pScene)
{
	MeshRenderComponent* mrc = GetComponent<MeshRenderComponent>("MeshRenderComponent");
	if (mrc) mrc->Render(pScene);
	return S_OK;
}

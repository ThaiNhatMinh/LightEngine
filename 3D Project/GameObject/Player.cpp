#include "pch.h"
#include "Player.h"

bool Player::Init(const tinyxml2::XMLElement * pData)
{
	return Actor::Init(pData);
}

void Player::PostInit(void)
{
	Actor::PostInit();
}

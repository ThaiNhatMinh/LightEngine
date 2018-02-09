#include "pch.h"

bool UIControl::Update(float dt)
{
	for (auto& el : m_Children) el->Update(dt);

	return true;
}

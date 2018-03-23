#include <pch.h>

StaticObject::StaticObject(ActorId id):Actor(id)
{
}

void StaticObject::PostInit(void)
{
	Actor::PostInit();

	m_MeshRender = std::unique_ptr<MeshRenderComponent>(RemoveComponent<MeshRenderComponent>(MeshRenderComponent::Name));
}

HRESULT StaticObject::VRender(Scene * pScene)
{
	//glEnable(GL_BLEND);
	m_MeshRender->Render(pScene);
	//glDisable(GL_BLEND);
	return S_OK;
}

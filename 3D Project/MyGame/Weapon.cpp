#include "pch.h"
#include <filesystem>
Weapon::Weapon(ActorId id):Actor(id)
{
}

bool Weapon::Init(const tinyxml2::XMLElement * pData)
{
	return Actor::Init(pData);
}

void Weapon::PostInit(void)
{
	Actor::PostInit();

	m_MeshRender = std::unique_ptr<MeshRenderComponent>(RemoveComponent<MeshRenderComponent>(MeshRenderComponent::Name));
	
}

HRESULT Weapon::VRender(Scene * pScene)
{
	//m_Context->m_pDebuger->DrawCoord(VGetGlobalTransform());
	m_MeshRender->Render(pScene);
	return E_NOTIMPL;
}

HRESULT Weapon::VOnUpdate(Scene *pScene, float elapsedMs)
{
	m_BoneTransform = m_ParentAnim->GetBoneTransform()[m_BoneID];


	return Actor::VOnUpdate(pScene, elapsedMs);
}

mat4 Weapon::VGetGlobalTransform()
{
	mat4 finalT = m_BoneTransform*m_SocketTransform;
	if (m_pParent) finalT = m_pParent->VGetGlobalTransform()*finalT;
	
	return finalT;
}

int Weapon::GetWeaponIndex()
{
	return m_Index;
}

mat4 & Weapon::GetSocketTransform()
{
	return m_SocketTransform;
}

void Weapon::LoadData(const WeaponResource & wr,LTBSocket* socket)
{
	m_BoneID = socket->m_iNode;
	m_SocketTransform = glm::rotate(mat4(), glm::radians(-90.0f), vec3(1, 0, 0))*socket->Transform;
	m_ParentAnim = m_pParent->GetComponent<AnimationComponent>(AnimationComponent::Name);
	m_Index = wr.index;
	ModelCache* pModel = m_Context->m_pResources->GetModel(wr.ModelFile.c_str());

	if (!pModel) return;

	vector<std::unique_ptr<SkeMesh>>& ve = pModel->pMeshs;
	for (size_t j = 0; j < ve.size(); j++)
	{
		const char* pTextureFile = wr.ModelTex.c_str();
		ve[j]->Tex = m_Context->m_pResources->GetTexture(pTextureFile);
		m_MeshRender->GetMeshList().push_back(pModel->pMeshs[j].get());
	}


}

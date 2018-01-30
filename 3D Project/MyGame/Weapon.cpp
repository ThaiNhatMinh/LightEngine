#include "pch.h"
#include <filesystem>
Weapon::Weapon(ActorId id):Actor(id)
{
}

bool Weapon::Init(const tinyxml2::XMLElement * pData)
{
	const tinyxml2::XMLElement* pInfo = pData->FirstChildElement("Info");
	if (pInfo)
	{
		//GViewAnimName = pInfo->Attribute("GViewAnimName");
		//PVFileName = pInfo->Attribute("PVModel");
		//WeaponSlot = pInfo->Int64Attribute("WeaponSlot",-1);
		const char* pName = pInfo->Attribute("Name");
		WeaponInfo = Game::LoadWeaponInfo(pName);
	}

	return Actor::Init(pData);
}

void Weapon::PostInit(void)
{
	m_ParentAnim = m_pParent->GetComponent<AnimationComponent>(AnimationComponent::Name);

	m_ParentAnim->SetBaseAnim(WeaponInfo.GViewAnimName);

	if (m_pParent->VGetTag() == "Player")
	{
		Player* pPlayer = static_cast<Player*>(m_pParent);
		auto socketlist = pPlayer->GetSockets();
		for (size_t i = 0; i < socketlist.size(); i++)
		{
			if (strcmp(socketlist[i].m_Name , "righthand-weapo")==0)
			{
				m_BoneID = socketlist[i].m_iNode;
				m_SocketTransform = glm::rotate(mat4(), glm::radians(-90.0f), vec3(1, 0, 0))*glm::rotate(mat4(), glm::radians(180.0f), vec3(0, 1, 0))*socketlist[i].Transform;
			}
		}
	}
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
mat4 & Weapon::GetSocketTransform()
{
	return m_SocketTransform;
}

const string & Weapon::GetPVFileName() {
	return WeaponInfo.PVModelFile;
}


int Weapon::GetWeaponIndex()
{
	assert(0);
	return WeaponInfo.index;
}

const WeaponResource & Weapon::GetWeaponInfo() const {
	return WeaponInfo;
}




int Weapon::GetWeaponSlot()
{
	return WeaponInfo.TargetSlot;
}
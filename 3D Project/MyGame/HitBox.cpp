#include "pch.h"

const char* HitBox::Name = "HitBox";
HitBox::HitBox()
{
}

HitBox::~HitBox()
{
}

bool HitBox::VInit(const tinyxml2::XMLElement * pData)
{
	return true;
}

tinyxml2::XMLElement * HitBox::VGenerateXml(tinyxml2::XMLDocument * p)
{
	return nullptr;
}

void HitBox::VPostInit(void)
{
	m_ParentAnimComponent = m_pOwner->GetComponent<AnimationComponent>(AnimationComponent::Name);

	auto meshrender = m_pOwner->GetComponent<MeshRenderComponent>(MeshRenderComponent::Name);

	if (meshrender)
	{
		m_Shape = new btCompoundShape(0);

		vector<std::unique_ptr<SkeNode>>& nodelist = meshrender->GetNodeList();
		for(size_t i=0; i<nodelist.size(); i++)
			if (nodelist[i]->m_Flag == 1)
			{
				ShapeBoxInfo boxInfo;

				vec3 size = nodelist[i]->m_BoundBox.Max - nodelist[i]->m_BoundBox.Min;
				vec3 pos = size / 2.0f + nodelist[i]->m_BoundBox.Min;
				btBoxShape* pBox = new btBoxShape(ToBtVector3(size / 2.0f));
				mat4 transform = glm::translate(glm::mat4(), pos);
				boxInfo.LocalTransform = transform;
				transform = nodelist[i]->m_GlobalTransform * transform;
				m_Shape->addChildShape(Mat4x4_to_btTransform(transform), pBox);
				
				
				boxInfo.index = i;
				boxInfo.size = size;
				

				m_List.push_back(boxInfo);
			}
		
		m_Body = new btCollisionObject();
		m_Body->setCollisionShape(m_Shape);
		m_Body->setUserPointer(this);
		m_Context->m_pPhysic->GetCollisionWorld()->addCollisionObject(m_Body, 0xffff,TYPE_HITBOX);
	}
}

void HitBox::VUpdate(float dt)
{
	
	m_Body->setWorldTransform(Mat4x4_to_btTransform(m_pOwner->VGetGlobalTransform()));
	for (size_t i = 0; i < m_List.size(); i++)
	{
		mat4 boneTransform = m_ParentAnimComponent->GetBoneTransform()[m_List[i].index];
		m_List[i].BoneTransform = boneTransform;
		m_Shape->updateChildTransform(i, Mat4x4_to_btTransform(boneTransform*m_List[i].LocalTransform));
	}
}

const char * HitBox::VGetName() const {
	return Name;
}

int HitBox::Inside(const vec3 & pos)
{
	int hitIndex = -1;
	float minsize=0xffffff;
	for (size_t i = 0; i < m_List.size(); i++)
	{
		vec3 center = m_List[i].LocalTransform[3] + m_List[i].BoneTransform[3];
		float distance = glm::length2(pos - center);
		if (distance < minsize)
		{
			minsize = distance;
			hitIndex = i;
		}
		
	}

	return hitIndex;
}

#pragma once


class HitBox:public ActorComponent
{
public:
	HitBox();
	~HitBox();
	static const char* Name;
	virtual bool VInit(const tinyxml2::XMLElement* pData);
	virtual tinyxml2::XMLElement* VGenerateXml(tinyxml2::XMLDocument*p);
	virtual void VPostInit(void);
	virtual void VUpdate(float dt);
	virtual const char *VGetName() const;;

	int Inside(const vec3& pos);
private:
	struct ShapeBoxInfo
	{
		mat4 BoneTransform;
		mat4 LocalTransform;
		vec3 size;
		int index; // index in skeleton
	};
private:
	
	std::vector<ShapeBoxInfo> m_List;
	btCompoundShape *m_Shape;
	btCollisionObject *m_Body;

	AnimationComponent* m_ParentAnimComponent;
};
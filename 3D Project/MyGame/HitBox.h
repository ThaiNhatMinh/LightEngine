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
	virtual const char *VGetName() const {
		return Name;
	};


private:
	
	std::vector<int> m_List;
	std::vector<mat4> m_TranslateList;
	btCompoundShape *m_Shape;
	btCollisionObject *m_Body;

	AnimationComponent* m_ParentAnimComponent;
};
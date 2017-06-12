#pragma once
#include "..\pch.h"


class AnimationComponent : public ActorComponent
{
private:
	vector<SkeNode*>	m_pSkeNodes;
	vector<Animation*>	m_pAnimList;
	vector<WeightBlend> m_WB;
	vector<FrameData> m_CurrentFrames;

	FrameData InterpolateFrame(AnimControl& control, const AnimNode& Anim, const vector<AnimKeyFrame>&);

	vector<mat4>		m_SkeTransform;

	AnimControl m_Default;

public:

	static const char* Name;
	virtual const char* VGetName() const { return Name; }
	AnimationComponent(void);
	virtual bool VInit(tinyxml2::XMLElement* pData);
	virtual tinyxml2::XMLElement* VGenerateXml(tinyxml2::XMLDocument*p) { return nullptr; };
	virtual void VUpdate(float deltaMs);


	const vector<mat4>& GetTransform();

};
#pragma once

#include "..\pch.h"


//class PlayerMotion;

// ----------------------
// Animation system version 2.0


class BaseAnim
{
	friend class PlayerMotion;
	friend class LTBFile;
	friend class SkeEnity;
private:
	vector<SkeNode*>	m_pSkeNodes;
	vector<Animation*> m_pAnimList;
	//float m_fTime;
	float m_fSpeed;
	bool m_bCanRelease;
	// current bone angle;
	float m_fAngle;
	/*	m_bOnAnimation value
	0 - ilde play animation	== no other animation so play idle
	1 - on playing animation
	2 - on blending animation

	*/
	GLuint m_bOnAnimation;

	// Time blend betwen 2 animation
	float m_fBlendTime;
	// FrameData for current frame. use for blending
	vector<FrameData> m_CurrentFrames;
	AnimControl m_Default;
	// this animation not repeat
	AnimControl m_More;

	vector<WeightBlend> m_WS;

protected:
	static FrameData InterpolateFrame(AnimControl& control , const AnimNode& Anim, const vector<AnimKeyFrame>&);
public:
	BaseAnim();
	~BaseAnim();
	void SetAnim(int a);
	void NextAnim() { m_Default.m_iCurrentAnim++; if (m_Default.m_iCurrentAnim == m_pAnimList.size()) m_Default.m_iCurrentAnim = 0; };
	void Update(float dt, JointList& skeleton);
	vector<Animation*>& Get() { return m_pAnimList; };
	vector<WeightBlend>& GetWS() { return m_WS; };
	void SetBlendTime(float a) { m_fBlendTime = a; }
	void AddAnimation(GLuint a);
	bool DeltaAngle(float dtAngle);
	AABB GetBV();
};
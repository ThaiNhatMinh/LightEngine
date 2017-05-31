#pragma once

#include "..\pch.h"

struct AnimKeyFrame
{
	unsigned int		m_Time;
	// A string of information about this key..
	std::string			m_pString;
};
struct FrameData
{
	vec3 m_Pos;
	quat m_Ort;
};


struct LTAJoint
{
	mat4 LocalTransform;
};
typedef std::vector<LTAJoint> JointList;

struct SkeNode
{
	string m_Name;
	GLint m_Index,m_ParentIndex;
	mat4 m_GlobalTransform;
	mat4 m_InvBindPose;
};

// animation data in one node
typedef vector<FrameData> AnimData;
struct AnimNode
{
	GLint Parent;
	AnimData Data;
};
struct Animation
{
	string Name;
	AABB m_BV;
	std::vector<AnimKeyFrame> KeyFrames;
	std::vector<AnimNode> AnimNodeLists;
};

//class PlayerMotion;
struct AnimControl
{
	int		KeyFrameID;
	float	m_fTime;
	GLuint	m_iCurrentAnim;
	GLuint	m_iCurrentFrame;
	bool	m_bActive;
};

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
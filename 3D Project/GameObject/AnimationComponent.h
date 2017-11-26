#pragma once
#include "pch.h"

// Animation system version 3.0


struct AnimKeyFrame
{
	unsigned int		m_Time;
	// A string of information about this key..
	std::string			m_pString;
};


struct SkeNode
{
	// Name of node
	string m_Name;
	// Index of Node 
	GLint m_Index;
	// Parent of node, -1 if root
	GLint m_ParentIndex;
	// transform to default vertex;
	mat4 m_GlobalTransform;
	// transform vertex to local coord;
	mat4 m_InvBindPose;

	GLuint m_Flag;
	// BoundBox OBB;
	AABB m_BoundBox;
};

// animation data in one node

struct FrameData
{
	vec3 m_Pos;
	quat m_Ort;
};
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

enum AnimationState
{
	ANIM_PLAYING,
	ANIM_TRANSITION,
	ANIM_STOP,
};

struct AnimControl
{
	int				KeyFrameID;
	float			m_fTime;
	GLuint			m_iCurrentAnim;
	GLuint			m_iCurrentFrame;
	AnimationState	m_State;
	bool			m_bFinished;
};

class AnimationComponent : public ActorComponent
{
private:
	vector<SkeNode*>	m_pSkeNodes;
	vector<Animation*>	m_pAnimList;
	vector<WeightBlend> m_WB;
	vector<FrameData>	m_CurrentFrames;

	
	vector<mat4>		m_SkeTransform;
	vector<mat4>		m_DbTransform;
	AnimControl			m_Control[2];
	float				m_fBlendTime;
	GLuint				m_iDefaultAnimation;
protected:
	FrameData			InterpolateFrame(AnimControl& control, const AnimNode& Anim, const vector<AnimKeyFrame>&);
	blendset			GetBlendSet(GLuint id);
	void				ResetControl(blendset bs, GLuint anim,AnimationState state);
	void				SendAnimationEvent(string data);
public:
	AnimationComponent(void);
	~AnimationComponent(void);

	static const char*	Name;
	virtual const char* VGetName() const { return Name; }
	
	virtual bool		VInit(const tinyxml2::XMLElement* pData);
	virtual void		VPostInit(void);
	virtual tinyxml2::XMLElement* VGenerateXml(tinyxml2::XMLDocument*p) { return nullptr; };
	virtual void		VUpdate(float deltaMs);

	void				DrawSkeleton(Debug& debug,const mat4& m);
	const vector<mat4>&	GetTransform();
	// Event 
	void				SetAnimationEvent(std::shared_ptr<const IEvent> pEvent);
	AABB				GetUserDimesion();

};
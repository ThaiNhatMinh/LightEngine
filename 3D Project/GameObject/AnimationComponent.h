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

class BaseAnimComponent : public ActorComponent
{
protected:
	vector<SkeNode*>	m_pSkeNodes;
	vector<Animation*>	m_pAnimList;
	vector<WeightBlend> m_WB;
	vector<FrameData>	m_CurrentFrames;


	vector<mat4>		m_SkeTransform;
	vector<mat4>		m_DbTransform;
	float				m_fBlendTime;
	GLuint				m_iDefaultAnimation;
protected:
	FrameData			InterpolateFrame(AnimControl& control, const AnimNode& Anim, const vector<AnimKeyFrame>&);
	GLint				FindAnimation(string name);
public:
	BaseAnimComponent() {};
	~BaseAnimComponent() {};
	virtual bool		VInit(const tinyxml2::XMLElement* pData);
	void				DrawSkeleton(const mat4& m);
	const vector<mat4>&	GetVertexTransform();
	const vector<mat4>&	GetBoneTransform();
	virtual AABB		GetUserDimesion()=0;
	virtual void		AnimEvent(const string&) = 0;
	virtual void		PlayAnimation(int anim,bool fromBaseAnim = true) = 0;
	virtual void		PlayDefaultAnimation() = 0;

};

class AnimationComponent : public BaseAnimComponent
{
private:
	
	AnimControl			m_Control[2];
	

	blendset			GetBlendSet(GLuint id);
	void				ResetControl(blendset bs, GLuint anim,AnimationState state);
	
protected:
	
public:
	AnimationComponent(void);
	~AnimationComponent(void);

	static const char*	Name;
	virtual const char* VGetName() const { return Name; }
	
	//virtual bool		VInit(const tinyxml2::XMLElement* pData);
	virtual void		VPostInit(void);
	virtual tinyxml2::XMLElement* VGenerateXml(tinyxml2::XMLDocument*p) { return nullptr; };
	virtual void		VUpdate(float deltaMs);

	
	// Event 
	void				SetAnimationEvent(std::shared_ptr<const IEvent> pEvent);
	virtual void		PlayAnimation(int anim,bool fromBaseAnim=true);
	virtual void		PlayDefaultAnimation();
	AABB				GetUserDimesion();
	virtual void		AnimEvent(const string&);
	void				SetBaseAnim(const string& name);

};

// Player view animation
class PVAnimationComponent : public BaseAnimComponent
{
private:

	AnimControl			m_Control;



	void				ResetControl(GLuint anim, AnimationState state);
public:
	PVAnimationComponent(void);
	~PVAnimationComponent(void) {};

	static const char*	Name;
	virtual const char* VGetName() const { return Name; }

	//virtual bool		VInit(const tinyxml2::XMLElement* pData);
	virtual void		VPostInit(void);
	virtual tinyxml2::XMLElement* VGenerateXml(tinyxml2::XMLDocument*p) { return nullptr; };
	virtual void		VUpdate(float deltaMs);


	// Event 
	void				SetAnimationEvent(std::shared_ptr<const IEvent> pEvent);
	virtual void		PlayAnimation(int anim, bool fromBaseAnim = true) {};
	virtual void		PlayDefaultAnimation() {};
	AABB				GetUserDimesion();
	virtual void		AnimEvent(const string&);
	mat4				GetRootTransform();

};
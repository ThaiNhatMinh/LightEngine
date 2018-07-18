#pragma once
#include <pch.h>

// Animation system version 3.0



enum AnimState
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
	AnimState		m_State;
	bool			m_bFinished;

	float			m_fBlendTime;
	bool			m_loop;
	float			m_speed;
};


class AnimationState
{
public:
	AnimationState(Animation* p);
	~AnimationState() {};


public:


	void			Update(float dt);

	int				KeyFrameID;
	float			m_fTime;
	Animation		*m_pAnim;
	GLuint			m_iCurrentFrame;
	bool			m_bFinished;

	bool			m_loop;
	float			m_speed;
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
	Debug*				m_pDebuger;
protected:
	FrameData			InterpolateFrame(AnimControl& control, const AnimNode& Anim, const vector<AnimKeyFrame>&);
	GLint				FindAnimation(string name);
public:
	

	BaseAnimComponent(void);
	~BaseAnimComponent() {};
	virtual bool		VInit(Context* pContext,const tinyxml2::XMLElement* pData);
	void				DrawSkeleton(const mat4& m);
	const vector<mat4>&	GetVertexTransform();
	const vector<mat4>&	GetBoneTransform();
	virtual AABB		GetUserDimesion()=0;
	virtual void		AnimEvent(const string&) = 0;
	//virtual void		PlayAnimation(int anim,bool fromBaseAnim = true) = 0;
	//virtual void		PlayAnimation(const string& anim, bool v = true) = 0;
	//virtual void		PlayDefaultAnimation() = 0;
	void				SetData(ModelCache* pModel);
	vector<Animation*>& GetAnimation() {
		return m_pAnimList;
	};

};

class AnimationComponent : public BaseAnimComponent
{
private:
	EventManager * m_pEventManager;
public:

	enum blendset
	{
		upper,
		fullbody
	};

	struct CharAnimControl :public AnimControl
	{
		blendset m_layer; // none, upper or lower
		bool operator==(const CharAnimControl& other)const
		{
			return m_layer == other.m_layer;
		}

	};
	AnimationComponent(void);
	~AnimationComponent(void);

	
	virtual const char* VGetName() const { return "AnimationComponent"; }
	
	virtual bool		VInit(Context* pContext, const tinyxml2::XMLElement* pData);
	virtual void		VPostInit(void);
	virtual tinyxml2::XMLElement* VGenerateXml(tinyxml2::XMLDocument*p) { return nullptr; };
	virtual void		VUpdate(float deltaMs);
	virtual void		VPostUpdate();
	
	// Event 
	void				SetAnimationEvent(std::shared_ptr<IEvent> pEvent);
	//virtual void		PlayAnimation(int anim,bool fromBaseAnim=true);
	//virtual void		PlayAnimation(const string& anim, bool v = true);
	//virtual void		PlayDefaultAnimation();
	AABB				GetUserDimesion();
	virtual void		AnimEvent(const string&);
	//void				SetBaseAnim(const string& name);
	void				Play(blendset layer,int anim, bool loop = false);

	void				SetBoneEdit(float yaw,float pitch);
	bool				IsFinish();
private:

	std::list<CharAnimControl>			m_Controls;


	blendset			GetBlendSet(GLuint id);
	void				ResetControl(CharAnimControl& control, GLuint anim, AnimState state);

	float				m_Yaw, m_Pitch;
	
protected:
	void				ComputerFrame(CharAnimControl & control, int i);
};

// Player view animation
class PVAnimationComponent : public BaseAnimComponent
{
private:

	AnimControl			m_Control;
	GLuint				m_iDefaultAnimation;


	void				ResetControl(GLuint anim, AnimState state);
public:
	PVAnimationComponent(void);
	~PVAnimationComponent(void) {};
	virtual const char* VGetName() const { return "PVAnimationComponent"; }

	//virtual bool		VInit(const tinyxml2::XMLElement* pData);
	virtual void		VPostInit(void);
	virtual tinyxml2::XMLElement* VGenerateXml(tinyxml2::XMLDocument*p) { return nullptr; };
	virtual void		VUpdate(float deltaMs);
	virtual void		VPostUpdate();

	// Event 
	void				SetAnimationEvent(std::shared_ptr<IEvent> pEvent);
	virtual void		PlayAnimation(int anim, bool loop = false);

	AABB				GetUserDimesion();
	virtual void		AnimEvent(const string&);
	mat4				GetRootTransform();
	bool				IsFinish();

};
#include "..\pch.h"


const char* AnimationComponent::Name = "AnimationComponent";

FrameData AnimationComponent::InterpolateFrame(AnimControl & control, const AnimNode & Anim, const vector<AnimKeyFrame>& KeyFrames)
{
	int frame0 = -1;
	int frame1 = -1;
	float t = 0.0f;

	if (control.m_iCurrentFrame == KeyFrames[control.KeyFrameID].m_Time)
	{
		frame1 = control.KeyFrameID;
		if (KeyFrames[frame1].m_pString.size() > 0)	
			SendAnimationEvent(KeyFrames[frame1].m_pString);
		return	Anim.Data[frame1];
	}
	else if (control.m_iCurrentFrame > KeyFrames[control.KeyFrameID].m_Time)
	{
		control.KeyFrameID++;
		if (control.KeyFrameID == KeyFrames.size())
		{
			control.m_iCurrentFrame = 0;
			control.KeyFrameID = 0;
			if (KeyFrames[control.KeyFrameID].m_pString.size() > 0)	SendAnimationEvent(KeyFrames[control.KeyFrameID].m_pString);
			control.m_bFinished = 1;
			return	Anim.Data[control.KeyFrameID];
		}
		frame0 = control.KeyFrameID - 1;
		frame1 = control.KeyFrameID;
	}
	else
	{
		frame0 = control.KeyFrameID - 1;
		frame1 = control.KeyFrameID;
	}

	t = (float)(control.m_iCurrentFrame - KeyFrames[frame0].m_Time) / (float)(KeyFrames[frame1].m_Time - KeyFrames[frame0].m_Time);

	if (t>1.0f || t<0.0f) return Anim.Data[control.KeyFrameID];
	FrameData frame;
	frame.m_Pos = glm::lerp(Anim.Data[frame0].m_Pos, Anim.Data[frame1].m_Pos, t);
	frame.m_Ort = glm::slerp(Anim.Data[frame0].m_Ort, Anim.Data[frame1].m_Ort, t);

	return frame;
}

blendset AnimationComponent::GetBlendSet(GLuint id)
{
	Animation* pAnim = m_pAnimList.at(id);
	
	if (pAnim->Name.find("walk") != string::npos || pAnim->Name.find("run") != string::npos || pAnim->Name.find("jump") != string::npos) return lower;

	if (pAnim->Name.find("hit") != string::npos || pAnim->Name.find("shoot") != string::npos || pAnim->Name.find("combo") != string::npos || pAnim->Name.find("bigshot") != string::npos || pAnim->Name.find("reload") != string::npos) return upper;

	return lower;

}

void AnimationComponent::ResetControl(blendset bs, GLuint anim, AnimationState state)
{
	m_Control[bs].m_fTime = 0;
	m_Control[bs].m_iCurrentAnim = anim;
	m_Control[bs].m_iCurrentFrame = 0;
	m_Control[bs].KeyFrameID = 0;
	m_Control[bs].m_State = state;
	m_Control[bs].m_bFinished = 0;
}

void AnimationComponent::SendAnimationEvent(string data)
{
	IEvent* pEvent = new EvtData_AnimationString(m_pOwner->GetId(), data);
	gEventManager()->VQueueEvent(pEvent);
}

AnimationComponent::AnimationComponent(void):m_iDefaultAnimation(0)
{
	m_Control[upper].m_fTime = 0;
	m_Control[upper].m_iCurrentAnim = 0;
	m_Control[upper].m_iCurrentFrame = 0;
	m_Control[upper].KeyFrameID = 0;
	m_Control[upper].m_State = ANIM_STOP;

	m_Control[lower].m_fTime = 0;
	m_Control[lower].m_iCurrentAnim = 0;
	m_Control[lower].m_iCurrentFrame = 0;
	m_Control[lower].KeyFrameID = 0;
	m_Control[lower].m_State = ANIM_PLAYING;

	m_fBlendTime = 0.3f;
}

AnimationComponent::~AnimationComponent(void)
{
	gEventManager()->VRemoveListener(MakeDelegate(this, &AnimationComponent::SetAnimationEvent), EvtData_SetAnimation::sk_EventType);
}

bool AnimationComponent::VInit(tinyxml2::XMLElement* pData)
{
	// load model
	tinyxml2::XMLElement* pModelNode = pData->FirstChildElement("Model");
	const char* pFileName = pModelNode->Attribute("File");

	ModelCache* pModel = gResources()->LoadModel(pFileName);

	if (!pModel)
	{
		E_ERROR("AnimationComponent can't load data.");
		return 0;
	}

	m_pSkeNodes = pModel->pSkeNodes;
	m_pAnimList = pModel->pAnims;
	m_WB = pModel->wb;
	m_SkeTransform.resize(m_pSkeNodes.size());
	m_CurrentFrames.resize(m_pSkeNodes.size());
	
	return true;
}

void AnimationComponent::VPostInit(void)
{
	gEventManager()->VAddListener(MakeDelegate(this, &AnimationComponent::SetAnimationEvent), EvtData_SetAnimation::sk_EventType);
}

void AnimationComponent::VUpdate(float deltaMs)
{
	if (!m_pAnimList.size()) return;


	if (m_Control[upper].m_State != ANIM_STOP)
	{
		m_Control[upper].m_fTime += deltaMs;
		m_Control[upper].m_iCurrentFrame = (GLuint)(m_Control[upper].m_fTime * 1000);
	}
	if (m_Control[lower].m_State != ANIM_STOP)
	{
		m_Control[lower].m_fTime += deltaMs;
		m_Control[lower].m_iCurrentFrame = (GLuint)(m_Control[lower].m_fTime * 1000);
	}


	Animation* animUpper = m_pAnimList[m_Control[upper].m_iCurrentAnim];
	Animation* animLower = m_pAnimList[m_Control[lower].m_iCurrentAnim];


	for (GLuint i = 0; i < animLower->AnimNodeLists.size(); i++)
	{
		// process upper 
		if (m_Control[upper].m_State == ANIM_TRANSITION && m_WB[upper].Blend[i])
		{
			if (m_Control[upper].m_fTime > m_fBlendTime)
			{
				m_Control[upper].m_State = ANIM_PLAYING;
				m_Control[upper].m_fTime = 0.0f;
			}
			else
			{
				float t = m_Control[upper].m_fTime / m_fBlendTime;
				m_CurrentFrames[i].m_Pos = glm::lerp(m_CurrentFrames[i].m_Pos, animUpper->AnimNodeLists[i].Data[0].m_Pos, t);
				m_CurrentFrames[i].m_Ort = glm::slerp(m_CurrentFrames[i].m_Ort, animUpper->AnimNodeLists[i].Data[0].m_Ort, t);
			}
		}
		else if (m_Control[upper].m_State == ANIM_PLAYING && m_WB[upper].Blend[i])
		{
			m_CurrentFrames[i] = InterpolateFrame(m_Control[upper], animUpper->AnimNodeLists[i], animUpper->KeyFrames);
		}

		// process lower -- default animation
		if (m_Control[lower].m_State == ANIM_TRANSITION && m_WB[lower].Blend[i])
		{
			if (m_Control[lower].m_fTime > m_fBlendTime)
			{
				m_Control[lower].m_State = ANIM_PLAYING;
				m_Control[lower].m_fTime = 0.0f;
			}
			else
			{
				float t = m_Control[lower].m_fTime / m_fBlendTime;
				m_CurrentFrames[i].m_Pos = glm::lerp(m_CurrentFrames[i].m_Pos, animLower->AnimNodeLists[i].Data[0].m_Pos, t);
				m_CurrentFrames[i].m_Ort = glm::slerp(m_CurrentFrames[i].m_Ort, animLower->AnimNodeLists[i].Data[0].m_Ort, t);
			}
		}
		else if (m_Control[lower].m_State == ANIM_PLAYING && m_WB[lower].Blend[i])
		{
			m_CurrentFrames[i] = InterpolateFrame(m_Control[lower], animLower->AnimNodeLists[i], animLower->KeyFrames);
		}
		



		mat4 m_TransformLocal;
		mat4 rotate = glm::toMat4(m_CurrentFrames[i].m_Ort);
		mat4 translate = glm::translate(mat4(), m_CurrentFrames[i].m_Pos);
		mat4 transform = translate*rotate;

		if (animLower->AnimNodeLists[i].Parent != -1) m_TransformLocal = m_SkeTransform[animLower->AnimNodeLists[i].Parent] * transform;
		else m_TransformLocal = transform;
		
		m_SkeTransform[i] = m_TransformLocal;


	}


	if (m_Control[upper].m_bFinished) ResetControl(upper, m_iDefaultAnimation, ANIM_PLAYING);
	if (m_Control[lower].m_bFinished) ResetControl(lower, m_iDefaultAnimation, ANIM_PLAYING);
}

const vector<mat4>& AnimationComponent::GetTransform()
{
	for (size_t i = 0; i < m_pSkeNodes.size(); i++)
	{
		m_SkeTransform[i] = m_SkeTransform[i] * m_pSkeNodes[i]->m_InvBindPose;
	}

	return m_SkeTransform;
}

void AnimationComponent::SetAnimationEvent(const IEvent * pEvent)
{
	const EvtData_SetAnimation* p = dynamic_cast<const EvtData_SetAnimation*>(pEvent);

	if (p->GetId() != m_pOwner->GetId()) return;

	GLuint animID = p->GetAnimation();

	blendset bs = GetBlendSet(animID);
	
	m_Control[bs].KeyFrameID = 0;
	m_Control[bs].m_iCurrentFrame = 0;
	m_Control[bs].m_iCurrentAnim = animID;
	m_Control[bs].m_fTime = 0.0f;				// restart time to zero
	if(bs==lower) m_Control[bs].m_State = ANIM_TRANSITION;    // Set state to transition to blend current frame of character to frame 0 of m_iCurrentAnim
	else m_Control[bs].m_State = ANIM_PLAYING;
	if (p->isDefault()) m_iDefaultAnimation = animID;

}

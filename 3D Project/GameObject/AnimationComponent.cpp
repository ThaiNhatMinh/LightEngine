#include "pch.h"

const char* AnimationComponent::Name = "AnimationComponent";
const char* PVAnimationComponent::Name = "PVAnimationComponent";

#pragma region BaseAnimComponent



void BaseAnimComponent::DrawSkeleton(const mat4& m)
{


	for (size_t i = 0; i < m_pSkeNodes.size(); i++)
	{
		int parentID = m_pSkeNodes[i]->m_ParentIndex;
		if (parentID != -1)
		{
			vec3 pos1 = m_DbTransform[i][3];
			vec3 pos2 = m_DbTransform[parentID][3];
			m_Context->m_pDebuger->DrawLine(pos1, pos2, vec3(1.0f, 1.0, 1.0f), m);
		}
		/*
		if (m_pSkeNodes[i]->m_Flag != 1) continue;
		vec3 v[8];
		m_pSkeNodes[i]->m_BoundBox.GenPoint(v);
		//mat4 mm = temp;
		mat4 temp = m*m_DbTransform[i];
		vec3 color;
		if (select == i) color = vec3(0, 1, 0);
		else color = vec3(1,1,1);
		if (i != 6) continue;
		debug.DrawLine(m_pSkeNodes[i]->m_BoundBox.Min, m_pSkeNodes[i]->m_BoundBox.Max, vec3(0, 1, 0), temp);

		debug.DrawLine(v[0], v[1], color, temp);
		debug.DrawLine(v[1], v[2], color, temp);
		debug.DrawLine(v[2], v[3], color, temp);
		debug.DrawLine(v[3], v[0], color, temp);

		debug.DrawLine(v[4], v[5], color, temp);
		debug.DrawLine(v[5], v[6], color, temp);
		debug.DrawLine(v[6], v[7], color, temp);
		debug.DrawLine(v[7], v[4], color, temp);

		debug.DrawLine(v[0], v[4], color, temp);
		debug.DrawLine(v[1], v[5], color, temp);
		debug.DrawLine(v[2], v[6], color, temp);
		debug.DrawLine(v[3], v[7], color, temp);

		vec3 pos1 = m_DbTransform[i][3];

		vec3 front = vec3(m_DbTransform[i][0]) + pos1;
		debug.DrawLine(pos1, front, vec3(1, 0, 0), m);
		front = 2.0f*vec3(m_DbTransform[i][1]) + pos1;
		debug.DrawLine(pos1, front, vec3(0, 1, 0), m);
		front = 4.0f*vec3(m_DbTransform[i][2]) + pos1;
		debug.DrawLine(pos1, front, vec3(0, 0, 1), m);
		*/
	}

}

const vector<mat4>& BaseAnimComponent::GetBoneTransform()
{
	return m_DbTransform;
}


const vector<mat4>& BaseAnimComponent::GetVertexTransform()
{
	return m_SkeTransform;
}



bool BaseAnimComponent::VInit(const tinyxml2::XMLElement* pData)
{
	if (!pData) return false;
	// load model
	const tinyxml2::XMLElement* pModelNode = pData->FirstChildElement("Model");
	const char* pFileName = pModelNode->Attribute("File");

	const tinyxml2::XMLElement* pAnimNode = pData->FirstChildElement("DefaultAnim");
	const char* pNameAnim = pAnimNode->Attribute("Anim");


	ModelCache* pModel = m_Context->m_pResources->GetModel(pFileName);

	if (!pModel)
	{
		E_ERROR("AnimationComponent can't load data.");
		return 0;
	}
	
	// We cannot assign so just coppy pointer
	for (size_t i = 0; i<pModel->pSkeNodes.size(); i++)
		m_pSkeNodes.push_back(pModel->pSkeNodes[i].get());
	for (size_t i = 0; i<pModel->pAnims.size(); i++)
		m_pAnimList.push_back(pModel->pAnims[i].get());

	m_WB = pModel->wb;
	m_SkeTransform.resize(m_pSkeNodes.size());
	m_CurrentFrames.resize(m_pSkeNodes.size());
	m_DbTransform.resize(m_pSkeNodes.size());

	m_iDefaultAnimation = FindAnimation(pNameAnim);


	return true;
}


FrameData BaseAnimComponent::InterpolateFrame(AnimControl & control, const AnimNode & Anim, const vector<AnimKeyFrame>& KeyFrames)
{
	int frame0 = -1;
	int frame1 = -1;
	float t = 0.0f;

	if (control.m_iCurrentFrame == KeyFrames[control.KeyFrameID].m_Time)
	{
		frame1 = control.KeyFrameID;
		if (KeyFrames[frame1].m_pString.size() > 0)	
			AnimEvent(KeyFrames[frame1].m_pString);
		return	Anim.Data[frame1];
	}
	else if (control.m_iCurrentFrame > KeyFrames[control.KeyFrameID].m_Time)
	{
		control.KeyFrameID++;
		if (control.KeyFrameID == KeyFrames.size())
		{
			control.m_iCurrentFrame = 0;
			control.KeyFrameID = 0;
			control.m_bFinished = 1;
			if (KeyFrames[control.KeyFrameID].m_pString.size() > 0)	AnimEvent(KeyFrames[control.KeyFrameID].m_pString);
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

GLint BaseAnimComponent::FindAnimation(string name)
{
	for (size_t i = 0; i<m_pAnimList.size(); i++)
		if (m_pAnimList[i]->Name == name)
		{
			return i;
		}


	return -1;
}

#pragma endregion

#pragma region AnimationComponent
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

void AnimationComponent::AnimEvent(const string& data)
{
	std::shared_ptr<const IEvent> pEvent(new EvtData_AnimationString(m_pOwner->GetId(), data));
	m_Context->m_pEventManager->VQueueEvent(pEvent);
}

void AnimationComponent::SetBaseAnim(const string& name )
{
	for(size_t i=0; i<m_pAnimList.size(); i++)
		if (m_pAnimList[i]->Name.find(name) != string::npos)
		{
			m_iDefaultAnimation = i;
			break;
		}
}

AnimationComponent::AnimationComponent(void)
{
	m_iDefaultAnimation = 0;
	m_Control[upper].m_fTime = 0;
	m_Control[upper].m_iCurrentAnim = 0;
	m_Control[upper].m_iCurrentFrame = 0;
	m_Control[upper].KeyFrameID = 0;
	m_Control[upper].m_State = ANIM_PLAYING;

	m_Control[lower].m_fTime = 0;
	m_Control[lower].m_iCurrentAnim = 0;
	m_Control[lower].m_iCurrentFrame = 0;
	m_Control[lower].KeyFrameID = 0;
	m_Control[lower].m_State = ANIM_PLAYING;

	m_fBlendTime = 0.3f;
}

AnimationComponent::~AnimationComponent(void)
{
	m_Context->m_pEventManager->VRemoveListener(MakeDelegate(this, &AnimationComponent::SetAnimationEvent), EvtData_SetAnimation::sk_EventType);
}


void AnimationComponent::VPostInit(void)
{
	m_Context->m_pEventManager->VAddListener(MakeDelegate(this, &AnimationComponent::SetAnimationEvent), EvtData_SetAnimation::sk_EventType);
	
	ResetControl(upper, m_iDefaultAnimation, ANIM_PLAYING);
	ResetControl(lower, m_iDefaultAnimation, ANIM_PLAYING);
}

void AnimationComponent::VUpdate(float deltaMs)
{
	if (!m_pAnimList.size()) return;
	m_Control[lower].m_fTime += deltaMs;
	m_Control[upper].m_fTime += deltaMs;

	if (m_Control[upper].m_State == ANIM_PLAYING)
	{
		
		m_Control[upper].m_iCurrentFrame = (GLuint)(m_Control[upper].m_fTime * 1000);
	}
	else if(m_Control[upper].m_State == ANIM_TRANSITION)
	{
		if (m_Control[upper].m_fTime > m_fBlendTime)
		{
			m_Control[upper].m_State = ANIM_PLAYING;
			m_Control[upper].m_fTime = 0.0f;
			m_Control[upper].m_iCurrentFrame = 0;
		}
	}

	if (m_Control[lower].m_State == ANIM_PLAYING)
	{
		m_Control[lower].m_iCurrentFrame = (GLuint)(m_Control[lower].m_fTime * 1000);
	}
	else if(m_Control[lower].m_State == ANIM_TRANSITION)
	{
		
		if (m_Control[lower].m_fTime > m_fBlendTime)
		{
			m_Control[lower].m_State = ANIM_PLAYING;
			m_Control[lower].m_fTime = 0.0f;
			m_Control[lower].m_iCurrentFrame = 0;
		}
	}

	
	Animation* animUpper = m_pAnimList[m_Control[upper].m_iCurrentAnim];
	Animation* animLower = m_pAnimList[m_Control[lower].m_iCurrentAnim];


	for (GLuint i = 0; i < animLower->AnimNodeLists.size(); i++)
	{
		// process upper 
		if (m_Control[upper].m_State == ANIM_TRANSITION && m_WB[upper].Blend[i])
		{
			float t = m_Control[upper].m_fTime / m_fBlendTime;
			m_CurrentFrames[i].m_Pos = glm::lerp(m_CurrentFrames[i].m_Pos, animUpper->AnimNodeLists[i].Data[0].m_Pos, t);
			m_CurrentFrames[i].m_Ort = glm::slerp(m_CurrentFrames[i].m_Ort, animUpper->AnimNodeLists[i].Data[0].m_Ort, t);
		}
		else if (m_Control[upper].m_State == ANIM_PLAYING && m_WB[upper].Blend[i])
		{
			m_CurrentFrames[i] = InterpolateFrame(m_Control[upper], animUpper->AnimNodeLists[i], animUpper->KeyFrames);
		}

		// process lower -- default animation
		else if (m_Control[lower].m_State == ANIM_TRANSITION && m_WB[lower].Blend[i])
		{
			float t = m_Control[lower].m_fTime / m_fBlendTime;
			m_CurrentFrames[i].m_Pos = glm::lerp(m_CurrentFrames[i].m_Pos, animLower->AnimNodeLists[i].Data[0].m_Pos, t);
			m_CurrentFrames[i].m_Ort = glm::slerp(m_CurrentFrames[i].m_Ort, animLower->AnimNodeLists[i].Data[0].m_Ort, t);
		}
		else if (m_Control[lower].m_State == ANIM_PLAYING && m_WB[lower].Blend[i])
		{
			
			m_CurrentFrames[i] = InterpolateFrame(m_Control[lower], animLower->AnimNodeLists[i], animLower->KeyFrames);
		}
		



		mat4 m_TransformLocal;
		mat4 rotate = glm::toMat4(m_CurrentFrames[i].m_Ort);
		mat4 translate = glm::translate(mat4(), m_CurrentFrames[i].m_Pos);
		mat4 transform = translate*rotate;

		if (animLower->AnimNodeLists[i].Parent != -1) m_TransformLocal = m_DbTransform[animLower->AnimNodeLists[i].Parent] * transform;
		else m_TransformLocal = transform;
		
		m_SkeTransform[i] = m_TransformLocal;
		m_DbTransform[i] = m_TransformLocal;
		m_SkeTransform[i] = m_SkeTransform[i] * m_pSkeNodes[i]->m_InvBindPose;


	}

	
	if (m_Control[upper].m_bFinished) ResetControl(upper, m_Control[upper].m_iCurrentAnim, ANIM_PLAYING);
	if (m_Control[lower].m_bFinished) ResetControl(lower, m_Control[lower].m_iCurrentAnim, ANIM_PLAYING);
}


void AnimationComponent::SetAnimationEvent(std::shared_ptr<const IEvent> pEvent)
{
	const EvtData_SetAnimation* p = dynamic_cast<const EvtData_SetAnimation*>(pEvent.get());

	if (p->GetId() != m_pOwner->GetId()) return;

	GLuint animID = p->GetAnimation();

	if (animID >= m_pAnimList.size()) return;
	blendset bs = GetBlendSet(animID);
	
	m_Control[bs].KeyFrameID = 0;
	m_Control[bs].m_iCurrentFrame = 0;
	m_Control[bs].m_iCurrentAnim = animID;
	m_Control[bs].m_fTime = 0.0f;				// restart time to zero
	if(bs==lower) m_Control[bs].m_State = ANIM_TRANSITION;    // Set state to transition to blend current frame of character to frame 0 of m_iCurrentAnim
	else m_Control[bs].m_State = ANIM_PLAYING;
	if (p->isDefault()) m_iDefaultAnimation = animID;

}

void AnimationComponent::PlayAnimation(int anim, bool fromBaseAnim)
{
	GLint animID = 0;
	

	//if (animID == -1) return;
	if(fromBaseAnim) animID = anim + m_iDefaultAnimation;
	else animID = anim;

	blendset bs = GetBlendSet(animID);
	
	if (m_Control[bs].m_iCurrentAnim == animID) return;
	
	ResetControl(bs, animID, ANIM_TRANSITION);
}

void AnimationComponent::PlayDefaultAnimation()
{
	//blendset bs = GetBlendSet(m_iDefaultAnimation);

	if (m_Control[lower].m_iCurrentAnim == m_iDefaultAnimation) return;
	ResetControl(lower, m_iDefaultAnimation, ANIM_TRANSITION);

	if (m_Control[upper].m_iCurrentAnim == m_iDefaultAnimation) return;
	ResetControl(upper, m_iDefaultAnimation, ANIM_TRANSITION);
}

AABB AnimationComponent::GetUserDimesion()
{
	if (m_Control[lower].m_iCurrentAnim<0 || m_Control[lower].m_iCurrentAnim>=m_pAnimList.size()) return AABB();
	return m_pAnimList[m_Control[lower].m_iCurrentAnim]->m_BV;
}


#pragma endregion

#pragma region PlayerViewAnimation
// Using to control PV Model


void PVAnimationComponent::ResetControl(GLuint anim, AnimationState state)
{
	m_Control.m_fTime = 0;
	m_Control.m_iCurrentAnim = anim;
	m_Control.m_iCurrentFrame = 0;
	m_Control.KeyFrameID = 0;
	m_Control.m_State = state;
	m_Control.m_bFinished = 0;
}


PVAnimationComponent::PVAnimationComponent(void)
{
	m_iDefaultAnimation = 0;
	m_Control.m_fTime = 0;
	m_Control.m_iCurrentAnim = 0;
	m_Control.m_iCurrentFrame = 0;
	m_Control.KeyFrameID = 0;
	m_Control.m_State = ANIM_PLAYING;
	m_fBlendTime = 0.3;
}


void PVAnimationComponent::VPostInit(void)
{
}

void PVAnimationComponent::VUpdate(float deltaMs)
{
	if (!m_pAnimList.size()) return;

	if (m_Control.m_State != ANIM_STOP)
	{
		m_Control.m_fTime += deltaMs;
		m_Control.m_iCurrentFrame = (GLuint)(m_Control.m_fTime * 1000);
	}

	Animation* anim = m_pAnimList[m_Control.m_iCurrentAnim];
	for (GLuint i = 0; i < anim->AnimNodeLists.size(); i++)
	{
		if (m_Control.m_State == ANIM_TRANSITION)
		{
			if (m_Control.m_fTime > m_fBlendTime)
			{
				m_Control.m_State = ANIM_PLAYING;
				m_Control.m_fTime = 0.0f;
			}
			else
			{
				float t = m_Control.m_fTime / m_fBlendTime;
				m_CurrentFrames[i].m_Pos = glm::lerp(m_CurrentFrames[i].m_Pos, anim->AnimNodeLists[i].Data[0].m_Pos, t);
				m_CurrentFrames[i].m_Ort = glm::slerp(m_CurrentFrames[i].m_Ort, anim->AnimNodeLists[i].Data[0].m_Ort, t);
			}
		}
		else if (m_Control.m_State == ANIM_PLAYING)
		{
			m_CurrentFrames[i] = InterpolateFrame(m_Control, anim->AnimNodeLists[i], anim->KeyFrames);
		}




		mat4 m_TransformLocal;
		mat4 rotate = glm::toMat4(m_CurrentFrames[i].m_Ort);
		mat4 translate = glm::translate(mat4(), m_CurrentFrames[i].m_Pos);
		mat4 transform = translate*rotate;

		if (anim->AnimNodeLists[i].Parent != -1) m_TransformLocal = m_DbTransform[anim->AnimNodeLists[i].Parent] * transform;
		else m_TransformLocal = transform;

		m_SkeTransform[i] = m_TransformLocal;
		m_DbTransform[i] = m_SkeTransform[i];
		m_SkeTransform[i] = m_SkeTransform[i] * m_pSkeNodes[i]->m_InvBindPose;


	}


	if (m_Control.m_bFinished) ResetControl(m_iDefaultAnimation, ANIM_PLAYING);

}

void PVAnimationComponent::SetAnimationEvent(std::shared_ptr<const IEvent> pEvent)
{
	const EvtData_SetAnimation* p = dynamic_cast<const EvtData_SetAnimation*>(pEvent.get());

	if (p->GetId() != m_pOwner->GetId()) return;

	GLuint animID = p->GetAnimation();

	if (animID >= m_pAnimList.size()) return;

	m_Control.KeyFrameID = 0;
	m_Control.m_iCurrentFrame = 0;
	m_Control.m_iCurrentAnim = animID;
	m_Control.m_fTime = 0.0f;				// restart time to zero
	m_Control.m_State = ANIM_PLAYING;
	if (p->isDefault()) m_iDefaultAnimation = animID;
}

AABB PVAnimationComponent::GetUserDimesion()
{
	if (m_Control.m_iCurrentAnim<0 || m_Control.m_iCurrentAnim >= m_pAnimList.size()) return AABB();
	return m_pAnimList[m_Control.m_iCurrentAnim]->m_BV;
}

void PVAnimationComponent::AnimEvent(const string&)
{
}

mat4 PVAnimationComponent::GetRootTransform()
{
	return m_DbTransform[0];
}
#pragma endregion
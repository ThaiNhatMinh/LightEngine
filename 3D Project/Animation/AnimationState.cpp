#include "pch.h"
#include "AnimationState.h"
#include "..\Utilities\LTBStruct.h"
#include "..\GameComponents\AnimatorComponent.h"
#include <glm/gtx/common.hpp>
Light::AnimationState::AnimationState()
{
	m_TransitionState = nullptr;
	m_bLoop = false;
	m_fTime = 0;
	m_iCurrentFrame = 0;
	KeyFrameID = 0;
	m_fSpeed = 1.0f;
	//m_CurrentFrame.resize(numNode);
	//m_SkeTransform.resize(numNode);
	//m_DbTransform.resize(numNode);


}
void Light::AnimationState::Transition(const std::string & name, bool loop)
{
	// need t check if current in transition
	m_TransitionState = nullptr;
	for (std::size_t i = 0; i < m_NextState.size(); i++)
	{
		if (m_NextState[i]->GetName() == name)
		{
			m_TransitionState = m_NextState[i];
			break;
		}
	}

	if (m_TransitionState == nullptr)
	{
		E_WARNING("Can't found animation state: %s, no link with: %s ", name.c_str(), m_Name.c_str());
		return;
	}

	m_TransitionState->m_bLoop = loop;


}

void Light::AnimationState::Update(float dt)
{
	m_fTime += dt * m_fSpeed;
	m_iCurrentFrame = m_fTime * 1000;

	if (m_TransitionState)
	{
		m_TransitionState->m_fTime += dt;
		m_TransitionState->m_iCurrentFrame = m_TransitionState->m_fTime * 1000;

		if (m_TransitionState->m_fTime > m_fTransitionTime)
			m_pOwner->SetCurrentState(m_TransitionState);
	}

	/*for (int i = 0; i < m_iNumNode; i++)
	{
		m_CurrentFrame[i] = ComputerFrame(i);

		glm::mat4 m_TransformLocal;
		glm::mat4 rotate = glm::toMat4(m_CurrentFrame[i].m_Ort);
		glm::mat4 translate = glm::translate(mat4(), m_CurrentFrame[i].m_Pos);
		glm::mat4 transform = translate * rotate;


		if (m_pSkeNodes[i].m_ParentIndex != -1) m_TransformLocal = m_DbTransform[m_pSkeNodes[i].m_ParentIndex] * transform;
		else m_TransformLocal = transform;

		m_SkeTransform[i] = m_TransformLocal;
		m_DbTransform[i] = m_TransformLocal;
		m_SkeTransform[i] = m_SkeTransform[i] * m_pSkeNodes[i].m_InvBindPose;

	}*/
}

const std::string & Light::AnimationState::GetName()
{
	return m_Name;
}

Light::FrameData Light::AnimationState::ComputerFrame(int i)
{
	FrameData final;
	FrameData frame = InterpolateFrame(this, m_pAnimData->AnimNodeLists[i], m_pAnimData->KeyFrames);
	if (m_TransitionState)
	{
		FrameData exxtra = InterpolateFrame(m_TransitionState, m_TransitionState->m_pAnimData->AnimNodeLists[i], m_TransitionState->m_pAnimData->KeyFrames);

		float t = m_TransitionState->m_fTime / m_fTransitionTime;

		final.m_Pos = glm::lerp(frame.m_Pos, exxtra.m_Pos, t);
		final.m_Ort = glm::slerp(frame.m_Ort, exxtra.m_Ort, t);

	}
	else final = frame;

	return final;	
}


//		0...10...20...30...40...50...60...70...80...90 ->> KeyFrames
//			   |		
//			m_iCurrentFrame
Light::FrameData Light::AnimationState::InterpolateFrame(AnimationState * state, const AnimNode & Anim, const vector<AnimKeyFrame>& KeyFrames)
{
	int frame0 = -1;
	int frame1 = -1;
	float t = 0.0f;

	if (state->m_iCurrentFrame == KeyFrames[state->KeyFrameID].m_Time)
	{
		frame1 = state->KeyFrameID;

		if (KeyFrames[frame1].m_pString.size() > 0)
			AnimEvent(KeyFrames[frame1].m_pString);

		return	Anim.Data[frame1];
	}
	else if (state->m_iCurrentFrame > KeyFrames[state->KeyFrameID].m_Time)
	{
		state->KeyFrameID++;

		if (state->KeyFrameID >= KeyFrames.size()) // check if finish
		{
			state->KeyFrameID = KeyFrames.size()-1;
			state->m_iCurrentFrame = KeyFrames[state->KeyFrameID].m_Time;
			state->OnFinish();
			if (KeyFrames[state->KeyFrameID].m_pString.size() > 0)	AnimEvent(KeyFrames[state->KeyFrameID].m_pString);
			return	Anim.Data[state->KeyFrameID];
		}
		frame0 = state->KeyFrameID - 1;
		frame1 = state->KeyFrameID;
	}
	else
	{
		frame0 = state->KeyFrameID - 1;
		frame1 = state->KeyFrameID;
	}

	t = (float)(state->m_iCurrentFrame - KeyFrames[frame0].m_Time) / (float)(KeyFrames[frame1].m_Time - KeyFrames[frame0].m_Time);

	if (t > 1.0f || t < 0.0f)
	{
		E_WARNING("Interpolate between frame is wrong: %f", t);
		return Anim.Data[state->KeyFrameID];
	}
	FrameData frame;
	frame.m_Pos = glm::lerp(Anim.Data[frame0].m_Pos, Anim.Data[frame1].m_Pos, t);
	frame.m_Ort = glm::slerp(Anim.Data[frame0].m_Ort, Anim.Data[frame1].m_Ort, t);

	return frame;
}

void Light::AnimationState::AnimEvent(const string & name)
{
	///E_DEBUG("Animation event: %s", name.c_str());

}

void Light::AnimationState::OnFinish()
{
	
	if (m_bLoop)
	{
		m_fTime = 0;
		KeyFrameID = 0;
		m_iCurrentFrame = 0;
		m_bFinish = 0;
	}
	else
	{
		m_bFinish = 1;
	}
}
void Light::AnimationState::SetSpeed(float speed)
{
	m_fSpeed = speed;
}

bool Light::AnimationState::IsFinish()
{
	return m_bFinish;
}


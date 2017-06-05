#include "..\pch.h"
#include "..\ResourceCache\LTBDef.h"
#include "..\ResourceCache\LTBFileLoader.h"

FrameData BaseAnim::InterpolateFrame(AnimControl& control, const AnimNode & Anim, const vector<AnimKeyFrame>& KeyFrames)
{
	int frame0 = -1;
	int frame1 = -1;
	float t = 0.0f;

	if (control.m_iCurrentFrame == KeyFrames[control.KeyFrameID].m_Time)
	{
		frame1 = control.KeyFrameID;
		//frame0 = control.KeyFrameID - 1;
		return	Anim.Data[frame1];
	}
	else if (control.m_iCurrentFrame > KeyFrames[control.KeyFrameID].m_Time)
	{
		control.KeyFrameID++;
		if (control.KeyFrameID == KeyFrames.size())
		{
			control.m_iCurrentFrame = 0;
			control.KeyFrameID = 0;
			return	Anim.Data[control.KeyFrameID];
		}
		frame0 = control.KeyFrameID-1;
		frame1 = control.KeyFrameID;
	}
	else
	{
		frame0 = control.KeyFrameID-1;
		frame1 = control.KeyFrameID;
	}

	t = (float)(control.m_iCurrentFrame - KeyFrames[frame0].m_Time) / (float)(KeyFrames[frame1].m_Time - KeyFrames[frame0].m_Time);
	
	if (t>1.0f || t<0.0f) return Anim.Data[control.KeyFrameID];
	FrameData frame;
	frame.m_Pos = Math::lerp(Anim.Data[frame0].m_Pos, Anim.Data[frame1].m_Pos, t);
	frame.m_Ort = Math::slerp(Anim.Data[frame0].m_Ort, Anim.Data[frame1].m_Ort, t);
	return frame;
}

BaseAnim::BaseAnim()
{
	m_bOnAnimation = 1;
	m_Default.m_fTime = 0;
	m_Default.m_iCurrentAnim = 0;
	m_Default.m_iCurrentFrame = 0;
	m_Default.KeyFrameID = 0;
	m_fSpeed = 1.0f / 30.0f;
	m_bCanRelease = true;
	m_fBlendTime = 0.2f;
	m_More.m_bActive = false;
	m_fAngle= 0.0f;
}

BaseAnim::~BaseAnim()
{

	if (!m_bCanRelease) return;
	for (GLuint i = 0; i < m_pAnimList.size(); i++)
		delete m_pAnimList[i];
}

void BaseAnim::SetAnim(int a)
{
	/*if (m_iCurrentAnim == jump) return;

	if (a != m_iCurrentAnim)
	{
		m_iCurrentAnim = a;
		//m_pPlayerMotion->m_iCurrentFrame = 0;
		m_bOnAnimation = 2;
		m_fTime = 0.0f;
		KeyFrameID = 0;
	}*/

	if (m_Default.m_iCurrentAnim == jump) return;

	if (a != m_Default.m_iCurrentAnim)
	{
		m_Default.m_iCurrentAnim = a;
		m_Default.m_fTime = 0.0f;
		m_Default.KeyFrameID = 0;
		m_bOnAnimation = 2;
	}
}

void BaseAnim::Update(float dt, JointList& skeleton)
{
	if (!m_pAnimList.size()) return;
	m_Default.m_fTime += dt;
	m_Default.m_iCurrentFrame = (GLuint)(m_Default.m_fTime * 1000);

	if (m_More.m_bActive)
	{
		m_More.m_fTime += dt;
		m_More.m_iCurrentFrame = (GLuint)(m_More.m_fTime * 1000);
	}

	Animation* anim = m_pAnimList[m_Default.m_iCurrentAnim];
	if (!m_CurrentFrames.size()) m_CurrentFrames.resize(anim->AnimNodeLists.size());
	if (m_bOnAnimation == 2) // check blending 
	{
		if (m_Default.m_fTime > m_fBlendTime)
		{
			m_bOnAnimation = 1;
			m_Default.m_fTime = 0.0f;
			m_Default.m_iCurrentFrame = 0;
		}
		else
		{
			float t = m_Default.m_fTime / m_fBlendTime;
			for (GLuint j = 0; j < anim->AnimNodeLists.size(); j++)
			{

				mat4 m_TransformLocal;

				if (m_More.m_bActive && m_WS[0].Blend[j])
				{
					Animation* anim2 = m_pAnimList[m_More.m_iCurrentAnim];
					FrameData frame = InterpolateFrame(m_More, anim2->AnimNodeLists[j], anim2->KeyFrames);

					mat4 transform = frame.m_Ort.ToMatrix();
					transform = transform.Inverse();
					transform.Translate(frame.m_Pos);
					/*if (m_fAngle != 0 && m_pSkeNodes[j]->m_Name == "M-bone Spine")
					{
						quat rot;
						rot.setRotateXAxis(m_fAngle);
						mat4 tss = rot.ToMatrix();
						tss,0, 0, 0);
						transform = transform*tss;
						//transform,m_CurrentFrames[i].m_Pos);
					}*/
					if (anim2->AnimNodeLists[j].Parent != -1)
					{
						m_TransformLocal = transform*skeleton[anim2->AnimNodeLists[j].Parent].LocalTransform;
					}
					else m_TransformLocal = transform;
				}
				else
				{
					AnimNode& Anim = anim->AnimNodeLists[j];
					FrameData frame;
					frame.m_Pos = Math::lerp(m_CurrentFrames[j].m_Pos, Anim.Data[0].m_Pos, t);
					frame.m_Ort = Math::slerp(m_CurrentFrames[j].m_Ort, Anim.Data[0].m_Ort, t);

					// Unrotate + translate transform
					// this matrix transfrom local to parent space 
					mat4 transform = frame.m_Ort.ToMatrix();
					transform = transform.Inverse();
					transform.Translate(frame.m_Pos);

					/*if (m_fAngle != 0 && m_pSkeNodes[j]->m_Name == "M-bone Spine")
					{
						quat rot;
						rot.setRotateXAxis(m_fAngle);
						mat4 tss = rot.ToMatrix();
						tss,0, 0, 0);
						transform = transform*tss;
						//transform,m_CurrentFrames[i].m_Pos);
					}*/

					if (Anim.Parent != -1)
					{
						m_TransformLocal = transform*skeleton[Anim.Parent].LocalTransform;
					}
					else m_TransformLocal = transform;
				}


				LTAJoint joint;
				//joint.InvBindPose = m_InvBindPose;
				joint.LocalTransform = m_TransformLocal;
				//joint.parent = m_pParent->m_Index;

				skeleton[j] = joint;
			}
			if (m_More.m_iCurrentFrame == 0) m_More.m_bActive = 0;
		}
		
	}
	else if (m_bOnAnimation == 1)
	{
		for (GLuint i = 0; i < anim->AnimNodeLists.size(); i++)
		{
			mat4 m_TransformLocal;
			

			if (m_More.m_bActive && m_WS[0].Blend[i])
			{
				Animation* anim2 = m_pAnimList[m_More.m_iCurrentAnim];
				m_CurrentFrames[i] = InterpolateFrame(m_More, anim2->AnimNodeLists[i], anim2->KeyFrames);

				mat4 transform = m_CurrentFrames[i].m_Ort.ToMatrix();
				transform = transform.Inverse();
				transform.Translate(m_CurrentFrames[i].m_Pos);
				
				/*if (m_fAngle != 0 && m_pSkeNodes[i]->m_Name == "M-bone Spine")
				{
					quat rot;
					rot.setRotateXAxis(m_fAngle);
					mat4 tss = rot.ToMatrix();
					tss,0, 0, 0);
					transform = transform*tss;
					//transform,m_CurrentFrames[i].m_Pos);
				}*/
				if (anim2->AnimNodeLists[i].Parent != -1)
				{
					m_TransformLocal = transform*skeleton[anim2->AnimNodeLists[i].Parent].LocalTransform;
				}
				else m_TransformLocal = transform;
			}
			else
			{
				m_CurrentFrames[i] = InterpolateFrame(m_Default, anim->AnimNodeLists[i], anim->KeyFrames);
				// Unrotate + translate transform
				// this matrix transfrom local to parent space 
				mat4 transform = m_CurrentFrames[i].m_Ort.ToMatrix();
				transform = transform.Inverse();
				transform.Translate(m_CurrentFrames[i].m_Pos);
				//m_TransformLocal = transform;
				/*if (m_fAngle != 0 && m_pSkeNodes[i]->m_Name=="M-bone Spine")
				{
					quat rot;
					rot.setRotateXAxis(m_fAngle);
					mat4 tss = rot.ToMatrix();
					tss,0, 0, 0);
					transform = transform*tss ;
					//transform,m_CurrentFrames[i].m_Pos);
				}*/
				if (anim->AnimNodeLists[i].Parent != -1)
				{
					m_TransformLocal = skeleton[anim->AnimNodeLists[i].Parent].LocalTransform*transform;
				}
				else m_TransformLocal = transform;
			}
			LTAJoint joint;
			//joint.InvBindPose = m_InvBindPose;
			joint.LocalTransform = m_TransformLocal;
			//joint.parent = m_pParent->m_Index;

			skeleton[i] = joint;
		}

		if (m_Default.m_iCurrentFrame == 0)
		{
			if (m_Default.m_iCurrentAnim == jump)
			{
				m_Default.m_iCurrentAnim = sniper + idle;
				m_bOnAnimation = 2;
			}
			m_Default.m_fTime = 0;
		}
		if (m_More.m_iCurrentFrame == 0) m_More.m_bActive = 0;
	}
}

void BaseAnim::AddAnimation(GLuint a)
{
	if (m_More.m_bActive) return;
	m_More.m_fTime = 0.0f;
	m_More.m_iCurrentAnim = a;
	m_More.m_iCurrentFrame = 0;
	m_More.m_bActive = 1;
	m_More.KeyFrameID = 0;
}

bool BaseAnim::DeltaAngle(float dtAngle)
{
	m_fAngle += dtAngle;
	if (m_fAngle > 45.0f || m_fAngle < -45.0f)
	{
		m_fAngle = 0.0;
		return 1;
	}
	return 0;
}

AABB BaseAnim::GetBV()
{
	return m_pAnimList[m_Default.m_iCurrentAnim]->m_BV;
}


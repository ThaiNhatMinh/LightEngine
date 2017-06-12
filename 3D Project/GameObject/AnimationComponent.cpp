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

AnimationComponent::AnimationComponent(void)
{
	m_Default.m_fTime = 0;
	m_Default.m_iCurrentAnim = jump;
	m_Default.m_iCurrentFrame = 0;
	m_Default.KeyFrameID = 0;
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

void AnimationComponent::VUpdate(float deltaMs)
{
	if (!m_pAnimList.size()) return;

	m_Default.m_fTime += deltaMs;
	m_Default.m_iCurrentFrame = (GLuint)(m_Default.m_fTime*1000);
	//cout << deltaMs << " " << m_Default.m_iCurrentFrame << endl;

	Animation* anim = m_pAnimList[m_Default.m_iCurrentAnim];


	for (GLuint i = 0; i < anim->AnimNodeLists.size(); i++)
	{
		mat4 m_TransformLocal;

		m_CurrentFrames[i] = InterpolateFrame(m_Default, anim->AnimNodeLists[i], anim->KeyFrames);
		// Unrotate + translate transform
		// this matrix transfrom local to parent space 
		mat4 rotate = glm::toMat4(m_CurrentFrames[i].m_Ort);
		//rotate = rotate.Inverse();
		mat4 translate = glm::translate(mat4(), m_CurrentFrames[i].m_Pos);
		mat4 transform = translate*rotate;

		if (anim->AnimNodeLists[i].Parent != -1)
		{
			m_TransformLocal = m_SkeTransform[anim->AnimNodeLists[i].Parent]* transform;
		}
		else m_TransformLocal = transform;
		
		m_SkeTransform[i] = m_TransformLocal;
	}


	if (m_Default.m_iCurrentFrame == 0)	m_Default.m_fTime = 0;
}

const vector<mat4>& AnimationComponent::GetTransform()
{
	for (size_t i = 0; i < m_pSkeNodes.size(); i++)
	{
		m_SkeTransform[i] = m_SkeTransform[i] * m_pSkeNodes[i]->m_InvBindPose;
	}

	return m_SkeTransform;
}

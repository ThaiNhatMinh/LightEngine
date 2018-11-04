#include "pch.h"
#include "AnimationLayer.h"
#include "..\ResourceManager\ResourceManager.h"

Light::AnimationLayer::AnimationLayer(int numNode):m_iNumNode(numNode)
{
}

bool Light::AnimationLayer::VSerialize(IContext * pContext, const tinyxml2::XMLElement * pData)
{
	auto pResources = pContext->GetSystem<resources::IResourceManager>();


	m_Name = pData->FirstChildElement("Name")->GetText();
	m_fWeight = std::atof(pData->FirstChildElement("Weight")->GetText());

	{
		auto pMaskNode = pData->FirstChildElement("Mask");
		int type = pMaskNode->Int64Attribute("type", 1.0);
		if (type == MASK_ZERO)
		{
			m_Masks.resize(m_iNumNode, 0);
		}
		else if (type == MASK_ONE)
		{
			m_Masks.resize(m_iNumNode, 1);
			m_Masks[0] = 0;
		}
		else if (type == MASK_DEFAULT)
		{
			for (auto pValue = pMaskNode->FirstChildElement(); pValue; pValue = pValue->NextSiblingElement())
			{
				m_Masks.push_back(std::atoi(pValue->GetText()));
			}

			if (m_Masks.size() != m_iNumNode)
			{
				m_Masks.resize(m_iNumNode, 0);
			}
		}
		else // type == MASK_FROMMODEL
		{
			auto pFile = pMaskNode->FirstChildElement("File")->GetText();
			auto layer = pMaskNode->FirstChildElement("File")->Attribute("name");

			auto Raw = static_cast<resources::LTRawData*>(pResources->VGetModel(pFile));
			
			if (Raw == nullptr) return false;
			
			int i;
			for (i = 0; i < Raw->wb.size(); i++)
			{
				if (!strcmp(Raw->wb[i].Name, layer)) break;
			}

			auto& wb = Raw->wb[i];
			for (int j = 0; j < m_iNumNode; j++)
				m_Masks.push_back(wb.Blend[j]);
		}
	}

	auto pStateNode = pData->FirstChildElement("AnimationState");
	if (!pStateNode) return false;

	for (auto pNode = pStateNode->FirstChildElement(); pNode; pNode = pNode->NextSiblingElement())
	{
		const char* name = pNode->FirstChildElement("Name")->GetText();
		const char* MotionFile = pNode->FirstChildElement("Motion")->GetText();
		const char* MotionName = pNode->FirstChildElement("Motion")->Attribute("name");
		float speed = std::atof(pNode->FirstChildElement("Speed")->GetText());

		auto Raw = static_cast<resources::LTRawData*>(pResources->VGetModel(MotionFile));
		Animation* pAnimData = GetAnimation(Raw, MotionName);
		if (pAnimData == nullptr) E_ERROR("Can't get animation: %s", MotionName);

		AnimationState state;
		state.m_pOwner = this;
		state.m_Name = name;
		state.m_pAnimData = pAnimData;
		state.m_fTransitionTime = pNode->DoubleAttribute("TransitionTime", 0.2);
		state.m_fSpeed = speed;

		m_AllState.push_back(state);
	}
	int i = 0;
	for (auto pNode = pStateNode->FirstChildElement(); pNode; pNode = pNode->NextSiblingElement())
	{
		for (auto pNexNode = pNode->FirstChildElement("Next"); pNexNode; pNexNode = pNexNode->NextSiblingElement("Next"))
		{
			const char* Next = pNexNode->GetText();
			auto pState = GetState(Next);
			m_AllState[i].m_NextState.push_back(pState);
		}
		i++;
	}

	const char* pEntryPoint;
	{
		auto pNode = pData->FirstChildElement("EntryPoint");
		pEntryPoint = pNode->GetText();
	}
	m_CurrentAnimation = GetState(pEntryPoint);
	m_CurrentAnimation->m_bLoop = true;

	return true;
}

tinyxml2::XMLElement * Light::AnimationLayer::VDeserialize(tinyxml2::XMLDocument * p)
{
	return nullptr;
}

void Light::AnimationLayer::Update(float dt)
{
	m_CurrentAnimation->Update(dt);
}

Light::FrameData Light::AnimationLayer::ComputerFrame(int i)
{
	FrameData frame = m_CurrentAnimation->ComputerFrame(i);
	frame.m_Pos *= m_fWeight * m_Masks[i];
	float t = m_fWeight * m_Masks[i];
	frame.m_Ort = glm::slerp(glm::quat(), frame.m_Ort, t);

	return frame;
}

Light::Animation * Light::AnimationLayer::GetAnimation(resources::LTRawData * pData, const std::string & name)
{
	for (std::size_t i = 0; i < pData->Anims.size(); i++)
	{
		if (pData->Anims[i].Name == name) return &pData->Anims[i];
	}

	return nullptr;
}


void Light::AnimationLayer::SetCurrentState(AnimationState * pState)
{
	// ? Any thing special need to do here?
	m_CurrentAnimation = pState;
}

Light::AnimationState * Light::AnimationLayer::GetCurrentState()
{
	return m_CurrentAnimation;
}

void Light::AnimationLayer::Play(const std::string & name, bool loop)
{
	if (m_CurrentAnimation->GetName() == name)
	{

	}else m_CurrentAnimation->Transition(name, loop);
}

const std::string & Light::AnimationLayer::GetName()
{
	return m_Name;
}


Light::AnimationState * Light::AnimationLayer::GetState(const std::string & name)
{
	for (auto& el : m_AllState)
		if (el.GetName() == name)
			return &el;

	return nullptr;
}
#include "pch.h"
#include "AnimatorComponent.h"
#include "MeshRenderComponent.h"
#include "..\Interface\IActor.h"
bool Light::AnimatorComponent::VSerialize(IContext * pContext, const tinyxml2::XMLElement * pData)
{
	auto pResources = pContext->GetSystem<resources::IResourceManager>();

	IMeshRenderComponent* pMeshRender = m_pOwner->GetComponent<IMeshRenderComponent>();
	if (pMeshRender == nullptr) return false;
	//m_pRenderModel = pMeshRender->m_pModel;

	
	// need to check if owner doesn't has mesh render

	auto pStateNode = pData->FirstChildElement("AnimationState");
	if (!pStateNode) return false;

	for (auto pNode = pStateNode->FirstChildElement(); pNode; pNode = pNode->NextSiblingElement())
	{
		const char* name = pNode->FirstChildElement("Name")->GetText();
		const char* MotionFile = pNode->FirstChildElement("Motion")->GetText();
		const char* MotionName = pNode->FirstChildElement("Motion")->Attribute("name");
		

		auto Raw = pResources->VGetRawModel(MotionFile);
		Animation* pAnimData = GetAnimation(Raw, MotionName);
		if (pAnimData == nullptr) E_ERROR("Can't get animation: %s", MotionName);

		AnimationState state(Raw->SkeNodes.size());
		state.m_pOwner = this;
		state.m_Name = name;
		state.m_pAnimData = pAnimData;
		state.m_pSkeNodes = Raw->SkeNodes.data();
		state.m_fTransitionTime = pNode->DoubleAttribute("TransitionTime", 0.2);

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

	const char* pEntryPoint = pData->FirstChildElement("EntryPoint")->GetText();

	m_CurrentAnimation = GetState(pEntryPoint);
	m_CurrentAnimation->m_bLoop = true;
	return true;
}

tinyxml2::XMLElement * Light::AnimatorComponent::VDeserialize(tinyxml2::XMLDocument * p)
{
	return nullptr;
}

void Light::AnimatorComponent::Play(const std::string & name, bool loop, float fadeinTime)
{
	m_CurrentAnimation->Transition(name, loop);
}

void Light::AnimatorComponent::SetCurrentState(AnimationState * pState)
{
	// ? Any thing special need to do here?
	m_CurrentAnimation = pState;
}

void Light::AnimatorComponent::VUpdate(float dt)
{
	m_CurrentAnimation->Update(dt);
	
}

void Light::AnimatorComponent::VPreRender(render::Material::MatrixParam & param)
{

	param[render::uSkeTransform] = m_CurrentAnimation->GetTransformMatrixs();
	param["numNode"] = (float*)m_CurrentAnimation->GetNumNodes();
}

Light::Animation * Light::AnimatorComponent::GetAnimation(LTRawData * pData, const std::string & name)
{
	for (std::size_t i = 0; i < pData->Anims.size(); i++)
	{
		if (pData->Anims[i].Name == name) return &pData->Anims[i];
	}

	return nullptr;
}

Light::AnimationState * Light::AnimatorComponent::GetState(const std::string & name)
{
	for (auto& el : m_AllState)
		if (el.GetName() == name)
			return &el;

	return nullptr;
}

int Light::AnimationState::GetNumNodes()
{
	return m_iNumNode;
}

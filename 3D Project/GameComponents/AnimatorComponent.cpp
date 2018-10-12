#include "pch.h"
#include "AnimatorComponent.h"
#include "MeshRenderComponent.h"
#include "..\Interface\IActor.h"
bool Light::AnimatorComponent::VSerialize(IContext * pContext, const tinyxml2::XMLElement * pData)
{
	auto pResources = pContext->GetSystem<resources::IResourceManager>();

	IMeshRenderComponent* pMeshRender = m_pOwner->GetComponent<IMeshRenderComponent>();
	// need to check if owner doesn't has mesh render
	if (pMeshRender == nullptr) return false;
	//m_pRenderModel = pMeshRender->m_pModel;
	auto pSkeleton = pData->FirstChildElement("Skeleton")->GetText();

	auto raw = pResources->VGetRawModel(pSkeleton);
	m_pSkeNodes = raw->SkeNodes.data();
	m_iNumNode = raw->SkeNodes.size();
	m_SkeTransform.resize(m_iNumNode);
	m_DbTransform.resize(m_iNumNode);
	m_CurrentFrame.resize(m_iNumNode);

	for (auto pNode = pData->FirstChildElement("AnimationLayer"); pNode; pNode = pNode->NextSiblingElement("AnimationLayer"))
	{
		AnimationLayer* layer = DEBUG_NEW AnimationLayer(m_iNumNode);
		layer->VSerialize(pContext, pNode);
		m_Layers.push_back(std::unique_ptr<AnimationLayer>(layer));
	}
	

	
	return true;
}

tinyxml2::XMLElement * Light::AnimatorComponent::VDeserialize(tinyxml2::XMLDocument * p)
{
	return nullptr;
}

void Light::AnimatorComponent::Play(const std::string & name, bool loop, const std::string& layer)
{
	for (std::size_t i = 0; i < m_Layers.size(); i++)
	{
		if (m_Layers[i]->GetName() == layer)
			m_Layers[i]->Play(name,loop);
	}
}


void Light::AnimatorComponent::VUpdate(float dt)
{
	for (auto& el : m_Layers) el->Update(dt);


	for (int i = 0; i < m_iNumNode; i++)
	{
		FrameData frame;
		for (auto& el : m_Layers)
		{
			FrameData data = el->ComputerFrame(i);
			frame.m_Pos += data.m_Pos;
			frame.m_Ort *= data.m_Ort;
		}

		m_CurrentFrame[i] = frame;

		glm::mat4 m_TransformLocal;
		glm::mat4 rotate = glm::toMat4(frame.m_Ort);
		glm::mat4 translate = glm::translate(mat4(), frame.m_Pos);
		glm::mat4 transform = translate * rotate;


		if (m_pSkeNodes[i].m_ParentIndex != -1) m_TransformLocal = m_DbTransform[m_pSkeNodes[i].m_ParentIndex] * transform;
		else m_TransformLocal = transform;

		m_SkeTransform[i] = m_TransformLocal;
		m_DbTransform[i] = m_TransformLocal;
		m_SkeTransform[i] = m_SkeTransform[i] * m_pSkeNodes[i].m_InvBindPose;
	}
	
}

void Light::AnimatorComponent::VPreRender(render::Material::MatrixParam & param)
{

	param[render::uSkeTransform] = glm::value_ptr(m_SkeTransform[0]);
	param["numNode"] = (float*)m_iNumNode;
}


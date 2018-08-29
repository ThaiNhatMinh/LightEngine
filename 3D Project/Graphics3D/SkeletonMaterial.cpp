#include "pch.h"
#include "SkeletonMaterial.h"

Light::render::SkeletonMaterial::SkeletonMaterial(IContext * pContext):m_pModelUniform(nullptr),m_pMVPUniform(nullptr)
{
	auto pRenderer = pContext->GetSystem<RenderDevice>();
	auto pResources = pContext->GetSystem<resources::IResourceManager>();

	auto pVertexShader = pResources->VGetVertexShader("Skeleton");
	auto pPixelShader = pResources->VGetPixelShader("Default");

	m_Pipeline = std::unique_ptr<Pipeline>(pRenderer->CreatePipeline(pVertexShader, pPixelShader));
	this->GetUniform();
}

void Light::render::SkeletonMaterial::Apply(RenderDevice * renderer, const float * model, const float * mvp)
{
	renderer->SetPipeline(m_Pipeline.get());
	if (m_pModelUniform) m_pModelUniform->SetAsMat4(model);
	m_pMVPUniform->SetAsMat4(mvp);

}

MaterialType Light::render::SkeletonMaterial::GetType()
{
	static std::size_t type = typeid(SkeletonMaterial).hash_code();
	return type;
}

void Light::render::SkeletonMaterial::GetUniform()
{
	assert(m_Pipeline != nullptr);
	m_pModelUniform = m_Pipeline->GetParam(uMODEL);
	m_pMVPUniform = m_Pipeline->GetParam(uMVP);
}

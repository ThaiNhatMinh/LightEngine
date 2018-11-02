#include "pch.h"
#include "BaseMaterial.h"

MaterialType Light::BaseMaterial::GetType()
{
	static std::size_t type = typeid(*this).hash_code();
	return type;
}

void Light::BaseMaterial::SetPipeline(render::Pipeline * pipeline)
{
	m_Pipeline.reset(pipeline);
	this->GetUniform();
}

Light::render::Pipeline * Light::BaseMaterial::GetPipeline()
{
	return m_Pipeline.get();
}

void Light::BaseMaterial::AddTexUnit(std::string name, render::TextureUnit unit)
{
	m_TextureUnits.push_back(std::pair<render::PipelineParam*, render::TextureUnit>(m_Pipeline->GetParam(name.c_str()), unit));
}

void Light::BaseMaterial::ClearTextureData()
{
	m_TextureUnits.clear();
}

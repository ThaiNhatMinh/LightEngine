#pragma once
#include "Material.h"
#include "..\Interface\Renderer.h"
namespace Light
{
	class BaseMaterial : public render::Material
	{
	public:
		virtual MaterialType GetType()override final;
		virtual void SetPipeline(render::Pipeline* pipeline)override final;
		virtual render::Pipeline* GetPipeline()override final;
		virtual void AddTexUnit(std::string name, render::TextureUnit unit)override final;
		virtual void ClearTextureData()override;
	};
}
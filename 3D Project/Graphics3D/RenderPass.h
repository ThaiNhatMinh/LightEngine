#pragma once
#include "Renderer.h"
#include "Material.h"

namespace Light
{
	namespace render
	{
		struct RenderPass
		{
			RenderDevice* pRenderer;
			Material* pGlobalMaterial = nullptr;
			DepthStencilState* pDepthStencilConfig;
		};
	}
}
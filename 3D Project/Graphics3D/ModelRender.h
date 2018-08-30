#pragma once
#include <vector>
#include <memory>
#include "..\Core\Log.h"
#include "RenderPass.h"

namespace Light
{
	namespace render
	{
		class Model
		{
		public:
			virtual ~Model(){};
			virtual void Draw(render::RenderPass * pass, const float * model, const float * mvp) = 0;
		};
	}
}
#pragma once
#include <vector>
#include <memory>
#include "..\Core\Log.h"

namespace Light
{
	namespace render
	{
		class Model
		{
		public:
			virtual ~Model(){};
			virtual void Draw(RenderDevice* renderer, const float * model, const float * mvp) = 0;
		};
	}
}
#pragma once
#include <vector>
#include <memory>
#include "Mesh.h"
#include "Material.h"

class Texture;
namespace Light
{
	namespace render
	{
		class Model
		{
		public:
			virtual ~Model() = default;
		};
	}
}
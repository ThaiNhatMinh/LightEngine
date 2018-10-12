#pragma once

// TinyXML 2
#include <tinyxml2/tinyxml2.h>
#include "..\Graphics3D\Material.h"
namespace Light
{
	namespace util
	{
		class Serialization
		{
		public:
			virtual ~Serialization() = default;

			virtual bool VSerialize(IContext*pContext,const tinyxml2::XMLElement * pData)=0;
			virtual tinyxml2::XMLElement* VDeserialize(tinyxml2::XMLDocument*p)=0;
		};

		class Updatable
		{
		public:
			virtual ~Updatable() = default;
			virtual void VUpdate(float dt) = 0;
		};

		class PreRenderable
		{
		public:
			virtual ~PreRenderable() = default;
			virtual void		VPreRender(render::Material::MatrixParam& param) = 0;
		};
	}
}
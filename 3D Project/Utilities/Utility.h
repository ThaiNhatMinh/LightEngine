#pragma once

// TinyXML 2
#include <tinyxml2.h>

namespace Light
{
	namespace util
	{
		class Serialization
		{
		public:
			virtual ~Serialization() = default;

			virtual bool VSerialize(const tinyxml2::XMLElement* pData)=0;
			virtual tinyxml2::XMLElement* VDeserialize(tinyxml2::XMLDocument*p)=0;
		};
	}
}
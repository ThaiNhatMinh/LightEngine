#include "pch.h"

Context* ISubSystem::m_Context = nullptr;

Context::Context()
{
	
	int errorID = ConfigData.LoadFile(ConfigFile);
	if (errorID)
	{
		E_ERROR("Failed to load engine config file: " + string(ConfigFile));
	}
}

Context::~Context()
{
	
}

tinyxml2::XMLElement * Context::GetElement(const char * p)
{
	tinyxml2::XMLElement * pData = nullptr;

	pData = ConfigData.FirstChildElement(p);

	return pData;
}

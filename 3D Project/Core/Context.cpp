#include "pch.h"

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
	E_WARNING("Context Release...");
}

ISubSystem * Context::GetSystem(const type_info& rtti)
{
	for (auto& el : m_Systems)
		if (el.first == rtti.name()) return el.second;

	return nullptr;
}

void Context::AddSystem(ISubSystem * system)
{
	//cout << typeid(*system).raw_name() << endl;
	for (auto& el : m_Systems)
		if (el.first == typeid(*system).name()) return;

	m_Systems.insert({ typeid(*system).name(),system });
}

tinyxml2::XMLElement * Context::GetElement(const char * p)
{
	tinyxml2::XMLElement * pData = nullptr;
	tinyxml2::XMLElement* pRoot = ConfigData.FirstChildElement("GameConfig");
	pData = pRoot->FirstChildElement(p);

	return pData;
}

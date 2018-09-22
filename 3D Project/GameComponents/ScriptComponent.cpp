#include "pch.h"
#include "ScriptComponent.h"
#include "..\Interface\IScriptManager.h"
Light::ScriptComponent::ScriptComponent(void)
{
}

bool Light::ScriptComponent::VSerialize(IContext * pContext, const tinyxml2::XMLElement * pData)
{
	m_LuaFile = pData->FirstChildElement("File")->GetText();

	auto pScripts = pContext->GetSystem<IScriptManager>();

	pScripts->BindComponent(this);

	return true;
}

tinyxml2::XMLElement * Light::ScriptComponent::VDeserialize(tinyxml2::XMLDocument * p)
{
	return nullptr;
}

const std::string & Light::ScriptComponent::VGetFile()
{
	return m_LuaFile;
}

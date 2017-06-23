#include "pch.h"


static const char* METATABLE_NAME = "BaseScriptComponentMetaTable";
const char* ScriptComponent::Name = "ScriptComponent";

ScriptComponent::ScriptComponent(void)
{
	m_StartFunction.AssignNil(gLuaState()->GetLuaState());
	m_FinishFunction.AssignNil(gLuaState()->GetLuaState());
	m_UpdateFunction.AssignNil(gLuaState()->GetLuaState());
}

ScriptComponent::~ScriptComponent(void)
{
	Finish();
}

bool ScriptComponent::VInit(tinyxml2::XMLElement * pData)
{
	tinyxml2::XMLElement* pScript = pData->FirstChildElement("Script");
	const char* pFileScript = pScript->Attribute("File");
	const char* pClassName = pScript->Attribute("class");
	m_LuaFile = pFileScript;
	m_ClassName = pClassName;


	return true;
}

void ScriptComponent::VPostInit(void)
{
	
	gLuaState()->VExecuteFile(m_LuaFile.c_str());
	CreateScriptObject();

	Start();
}

void ScriptComponent::VUpdate(float dt)
{
	Update(dt);
}

tinyxml2::XMLElement * ScriptComponent::VGenerateXml(tinyxml2::XMLDocument*p)
{
	return nullptr;
}

void ScriptComponent::CreateScriptObject()
{

	LuaPlus::LuaObject metaTableObj = gLuaState()->GetGlobalVars().Lookup(METATABLE_NAME);
	assert(!metaTableObj.IsNil());

	LuaPlus::LuaObject luaInstance = gLuaState()->GetGlobalVars().GetByName(m_ClassName.c_str());
	if(luaInstance.IsNil()) 
		luaInstance = gLuaState()->GetGlobalVars().CreateTable(m_ClassName.c_str());
	//luaInstance.AssignNewTable(gLuaState()->GetLuaState());
	// assign the C++ instance pointer to the lua instance
	luaInstance.SetLightUserData("__object", this);
	// assign the metatable to the new Lua instance table
	luaInstance.SetMetaTable(metaTableObj);

	// transform component functions
	//luaInstance.RegisterObjectDirect("GetActorId", this, &ScriptComponent::GetActorID);

	LuaPlus::LuaObject temp = luaInstance.GetByName("Start");
	if (temp.IsFunction()) 	m_StartFunction = temp;

	temp = luaInstance.GetByName("Finish");
	if (temp.IsFunction()) m_FinishFunction = temp;

	temp = luaInstance.GetByName("Update");
	if (temp.IsFunction()) m_UpdateFunction = temp;

	m_scriptObject = luaInstance;

}

void ScriptComponent::RegisterScriptFunctions(void)
{
	// create the metatable
	LuaPlus::LuaObject metaTableObj = gLuaState()->GetGlobalVars().CreateTable(METATABLE_NAME);
	metaTableObj.SetObject("__index", metaTableObj);

	// transform component functions
	metaTableObj.RegisterObjectDirect("GetActorId", (ScriptComponent*)0, &ScriptComponent::GetActorID);
	metaTableObj.RegisterObjectDirect("Start", (ScriptComponent*)0, &ScriptComponent::Start);
	metaTableObj.RegisterObjectDirect("Finish", (ScriptComponent*)0, &ScriptComponent::Finish);
	//metaTableObj.RegisterObjectDirect("Update", (ScriptComponent*)0, &ScriptComponent::Update);
	
}

void ScriptComponent::UnregisterScriptFunctions(void)
{
	LuaPlus::LuaObject metaTableObj = gLuaState()->GetGlobalVars().Lookup(METATABLE_NAME);
	if (!metaTableObj.IsNil())
		metaTableObj.AssignNil(gLuaState()->GetLuaState());
}

void ScriptComponent::Start()
{
	if (!m_StartFunction.IsNil())
	{
		LuaPlus::LuaFunction<void> CallBack(m_StartFunction);
		CallBack(m_scriptObject);
	}
}

void ScriptComponent::Update(float dt)
{
	if (!m_UpdateFunction.IsNil())
	{
		LuaPlus::LuaFunction<void> CallBack(m_UpdateFunction);
		CallBack(m_scriptObject,dt);
	}
}

void ScriptComponent::Finish()
{
	if (!m_FinishFunction.IsNil())
	{
		LuaPlus::LuaFunction<void> CallBack = m_FinishFunction;
		CallBack(m_scriptObject);
	}
}

ActorId ScriptComponent::GetActorID()
{
	ActorId id = m_pOwner->GetId();
	return id;
}

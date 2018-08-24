#pragma once
#include <pch.h>

// C++ class using for Lua Script

class ScriptComponent : public ActorComponent
{
protected:
	string m_LuaFile;
	string m_ClassName;
	LuaPlus::LuaObject m_StartFunction;
	LuaPlus::LuaObject m_FinishFunction;
	LuaPlus::LuaObject m_UpdateFunction;
	LuaPlus::LuaObject m_scriptObject;
public:

	static const char* Name;
	virtual const char* VGetName() const { return Name; }

	ScriptComponent(void);
	virtual ~ScriptComponent(void);
	virtual bool VInit(tinyxml2::XMLElement* pData);
	virtual void VPostInit(void);
	virtual void VUpdate(float dt);
	virtual tinyxml2::XMLElement* VGenerateXml(tinyxml2::XMLDocument*p);

	void CreateScriptObject();
	static void RegisterScriptFunctions(void);
	static void UnregisterScriptFunctions(void);

public:
	// Call before game loop begin
	void	Start();
	void	Update(float dt);
	void	Finish();
	ActorId GetActorID();
};
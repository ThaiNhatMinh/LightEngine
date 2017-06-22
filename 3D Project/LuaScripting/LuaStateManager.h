#pragma once

#include <pch.h>

class LuaStateManager : public IScriptManager, public Singleton<LuaStateManager>
{
	LuaPlus::LuaState* m_pLuaState;
	std::string m_lastError;

public:

	
	// Singleton functions
	void onStartUp(void);
	void onShutDown(void);
	

	// IScriptManager interface
	virtual bool VInit(void) override;
	virtual void VExecuteFile(const char* resource) override;
	virtual void VExecuteString(const char* str) override;

	LuaPlus::LuaObject GetGlobalVars(void);
	LuaPlus::LuaState* GetLuaState(void) const;

	// public helpers
	LuaPlus::LuaObject CreatePath(const char* pathString, bool toIgnoreLastElement = false);
	void ConvertVec3ToTable(const vec3& vec, LuaPlus::LuaObject& outLuaTable) const;
	void ConvertTableToVec3(const LuaPlus::LuaObject& luaTable, vec3& outVec3) const;

private:
	void SetError(int errorNum);
	void ClearStack(void);

};

LuaStateManager* gLuaState();
#pragma once
#include "..\Interface\IScriptManager.h"
#include "..\Interface\IEventManager.h"
#include "ScriptEventListener.h"
#include <sol.hpp>
#include <map>
namespace Light
{
	class LuaScriptManager : public IScriptManager
	{
	private:

		struct LuaState
		{
			IScriptComponent* m_Cmponent;
			sol::state m_Lua;
			sol::load_result m_Script;
			IActor* m_pActor;
			sol::function Update;
			std::vector<std::unique_ptr<EventListener>> m_Listener;
		};

	public:
		LuaScriptManager(IContext* pContext);
		~LuaScriptManager();
		virtual void		BindComponent(IScriptComponent*);
		virtual void		Start();
		virtual const char* VGetName();
		virtual void		Update(float dt)override;
	private:
		void BaseBinding(LuaState* pState);

	private:
		
		std::vector<std::unique_ptr<LuaState>> m_Lists;

		IEventManager* m_pEventManager;
	};
}
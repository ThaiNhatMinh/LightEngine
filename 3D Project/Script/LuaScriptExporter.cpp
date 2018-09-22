#include "pch.h"
#include "LuaScriptExporter.h"
#include "ComponentBinding.h"

#include "..\Interface\IActor.h"
Light::LuaScriptManager::LuaScriptManager(IContext * pContext)
{
	pContext->VAddSystem(this);
	m_pEventManager = pContext->GetSystem<IEventManager>();

}

Light::LuaScriptManager::~LuaScriptManager()
{
	for (auto&el : m_Lists)
	{
		for (auto& el2 : el->m_Listener)
		{
			
		}
	}
}

void Light::LuaScriptManager::BindComponent(IScriptComponent *pComponent)
{
	LuaState* state = new LuaState;
	state->m_Lua = sol::state();
	state->m_Cmponent = pComponent;
	state->m_pActor = pComponent->GetOwner();
	state->m_Lua.open_libraries(sol::lib::package, sol::lib::base, sol::lib::math);
	state->m_Lua.require("glm", sol::c_call<decltype(&GLMBinding), &GLMBinding>);
	state->m_Lua.require("Event", sol::c_call<decltype(&EventBinding), &EventBinding>);
	state->m_Lua.require("Input", sol::c_call<decltype(&InputBinding), &InputBinding>);
	
	BaseBinding(state);

	
	
	
	state->m_Script = state->m_Lua.load_file(pComponent->VGetFile());
	
	m_Lists.push_back(std::unique_ptr<LuaState>(state));
}

void Light::LuaScriptManager::Start()
{
	for (std::size_t i = 0; i < m_Lists.size(); i++)
		if (m_Lists[i]->m_Script.valid())
		{
			m_Lists[i]->m_Script();
		}
		else E_ERROR("Error on run lua script");
}

const char * Light::LuaScriptManager::VGetName()
{
	return typeid(IScriptManager).name();
}

void Light::LuaScriptManager::BaseBinding(LuaState* pState)
{
	sol::state& lua = pState->m_Lua;

	sol::table module = lua.create_table();

	module.new_usertype<TransformComponent>("Transform",
		sol::constructors<>(),
		"GetFront", &TransformComponent::GetFront,
		"GetRight", &TransformComponent::GetRight,
		"GetUp", &TransformComponent::GetUp,
		"GetPos", &TransformComponent::GetPos,
		"SetPos", &TransformComponent::SetPos,
		"SetTransform", &TransformComponent::SetTransform);


	lua["Log"] = [](std::string format)
	{
		std::cout << format;
	};

	lua["GetComponent"] = [this, pState, &lua](std::string name)
	{
		if (name == "Transform")
		{
			auto r = (TransformComponent*)(pState->m_pActor->VGetComponent(TransformComponent::StaticType));
			return sol::make_object(lua, r);
		}
	};

	lua["RegisterEvent"] = [this, pState, &lua](size_t eventType, sol::function callback)
	{
		EventListener* pListener = nullptr;
		if (eventType == events::EvtKeyEvent::StaticType)
		{
			auto* P = DEBUG_NEW ScriptEventListener<events::EvtKeyEvent>();
			m_pEventManager->VAddListener(DEBUG_NEW EventDelegate<ScriptEventListener<events::EvtKeyEvent>>(P, &ScriptEventListener<events::EvtKeyEvent>::OnEvent), events::EvtKeyEvent::StaticType);
			pListener = P;
		}
		else if (eventType == events::EvtDestroyActor::StaticType)
		{
			auto* P = DEBUG_NEW ScriptEventListener<events::EvtDestroyActor>();
			m_pEventManager->VAddListener(DEBUG_NEW EventDelegate<ScriptEventListener<events::EvtDestroyActor>>(P, &ScriptEventListener<events::EvtDestroyActor>::OnEvent), events::EvtDestroyActor::StaticType);
			pListener = P;
		}
		else if (eventType == events::EvtNewActor::StaticType)
		{
			auto* P = DEBUG_NEW ScriptEventListener<events::EvtNewActor>();
			m_pEventManager->VAddListener(DEBUG_NEW EventDelegate<ScriptEventListener<events::EvtNewActor>>(P, &ScriptEventListener<events::EvtNewActor>::OnEvent), events::EvtNewActor::StaticType);
			pListener = P;
		}
		else if (eventType == events::EvtMoveActor::StaticType)
		{
			auto* P = DEBUG_NEW ScriptEventListener<events::EvtMoveActor>();
			m_pEventManager->VAddListener(DEBUG_NEW EventDelegate<ScriptEventListener<events::EvtMoveActor>>(P, &ScriptEventListener<events::EvtMoveActor>::OnEvent), events::EvtMoveActor::StaticType);
			pListener = P;
		}

		if (pListener == nullptr)
		{
			E_ERROR("Can't find event type: %d", eventType);
			return;
		}
		pListener->callback = std::move(callback);
		pListener->type = eventType;
		pState->m_Listener.push_back(std::unique_ptr<EventListener>(pListener));
	};
}

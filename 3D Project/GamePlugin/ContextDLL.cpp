#include "pch.h"
#include "ContextDLL.h"

ContextDLL::ContextDLL(Context * context):m_Context(context)
{
}

IConsole * ContextDLL::GetConsole()
{
	return nullptr;
}

IPhysic * ContextDLL::GetPhysic()
{
	return nullptr;
}

IResources * ContextDLL::GetResources()
{
	return nullptr;
}

IRenderer * ContextDLL::GetRenderer()
{
	return nullptr;
}

ISystemUI * ContextDLL::GetSystemUI()
{
	return nullptr;
}

IWindows * ContextDLL::GetWindows()
{
	return nullptr;
}

IDebugger * ContextDLL::GetDebugger()
{
	return nullptr;
}

IEventManager * ContextDLL::GetEventManager()
{
	return nullptr;
}

IInput * ContextDLL::GetInput()
{
	return nullptr;
}

IEffectSystem * ContextDLL::GetEffectSystem()
{
	return nullptr;
}

IFactory * ContextDLL::GetFactory()
{
	return nullptr;
}

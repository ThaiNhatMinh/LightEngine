#include <pch.h>


//---------------------------------------------------------------------------------------------------------------------
// This is the C++ listener proxy for script event listeners.  It pairs a single event type with a Lua callback 
// function.  Note that this event can be defined in C++ or Lua.  It may also be sent from C++ or Lua.
// 
// The Lua callback function should take in a table with the event data.  The return value is ignored.
// function Callback(eventData)
// 
// Chapter 12, page 384
//---------------------------------------------------------------------------------------------------------------------
class ScriptEventListener
{
	EventType m_eventType;
	LuaPlus::LuaObject m_scriptCallbackFunction;

public:
	explicit ScriptEventListener(const EventType& eventType, const LuaPlus::LuaObject& scriptCallbackFunction);
	~ScriptEventListener(void);
	EventListenerDelegate GetDelegate(void) { return MakeDelegate(this, &ScriptEventListener::ScriptEventDelegate); }
	void ScriptEventDelegate(const IEvent* pEventPtr);
};

//---------------------------------------------------------------------------------------------------------------------
// This class manages the C++ ScriptListener objects needed for script event listeners.
//---------------------------------------------------------------------------------------------------------------------
class ScriptEventListenerManager
{
	typedef std::set<ScriptEventListener*> ScriptEventListenerSet;
	ScriptEventListenerSet m_listeners;

public:
	~ScriptEventListenerManager(void);
	void AddListener(ScriptEventListener* pListener);
	void DestroyListener(ScriptEventListener* pListener);
};

//---------------------------------------------------------------------------------------------------------------------
// Prototypes for the functions to export.                              - Chapter 12, page 368
//---------------------------------------------------------------------------------------------------------------------
class InternalScriptExports
{
	static ScriptEventListenerManager* s_pScriptEventListenerMgr;

public:
	// initialization
	static bool Init(void);
	static void Destroy(void);
	
	// resource loading
	static bool LoadAndExecuteScriptResource(const char* scriptResource);

	// actors
	static int CreateActor(const char* actorArchetype, LuaPlus::LuaObject luaPosition, LuaPlus::LuaObject luaYawPitchRoll);

	// event system
	static unsigned long RegisterEventListener(EventType eventType, LuaPlus::LuaObject callbackFunction);
	static void RemoveEventListener(unsigned long listenerId);
	static bool QueueEvent(EventType eventType, LuaPlus::LuaObject eventData);
	static bool TriggerEvent(EventType eventType, LuaPlus::LuaObject eventData);

	// process system
	static void AttachScriptProcess(LuaPlus::LuaObject scriptProcess);

	// math
	//static float GetYRotationFromVector(LuaPlus::LuaObject vec3);
	//static float WrapPi(float wrapMe);
	//static LuaPlus::LuaObject GetVectorFromRotation(float angleRadians);

	// misc
	static void LuaLog(LuaPlus::LuaObject text);
	static unsigned long GetTickCount(void);

	// physics
	static void ApplyForce(LuaPlus::LuaObject normalDir, float force, int actorId);
	static void ApplyTorque(LuaPlus::LuaObject axis, float force, int actorId);

private:
	static ScriptEvent* BuildEvent(EventType eventType, LuaPlus::LuaObject& eventData);
};

ScriptEventListenerManager* InternalScriptExports::s_pScriptEventListenerMgr = NULL;

ScriptEventListenerManager::~ScriptEventListenerManager(void)
{
	for (auto it = m_listeners.begin(); it != m_listeners.end(); it++)
	{
		ScriptEventListener* pListener = (*it);
		delete pListener;
	}
	m_listeners.clear();
}

void ScriptEventListenerManager::AddListener(ScriptEventListener * pListener)
{
	m_listeners.insert(pListener);
}

void ScriptEventListenerManager::DestroyListener(ScriptEventListener * pListener)
{
	auto it = m_listeners.find(pListener);
	if (it != m_listeners.end())
	{
		m_listeners.erase(it);
		delete (*it);
	}
	else
	{
		E_ERROR("Couldn't find script listener in set; this will probably cause a memory leak");
	}
	
}

ScriptEventListener::ScriptEventListener(const EventType & eventType, const LuaPlus::LuaObject & scriptCallbackFunction):m_eventType(eventType),m_scriptCallbackFunction(scriptCallbackFunction)
{
}

ScriptEventListener::~ScriptEventListener(void)
{
	gEventManager()->VRemoveListener(GetDelegate(), m_eventType);

}

void ScriptEventListener::ScriptEventDelegate(const IEvent * pEventPtr)
{
	assert(m_scriptCallbackFunction.IsFunction());  // this should never happen since it's validated before even creating this object

	// call the Lua function
	const ScriptEvent* pScriptEvent = dynamic_cast<const ScriptEvent*>(pEventPtr);
	ScriptEvent* p = (ScriptEvent*)pScriptEvent;
	LuaPlus::LuaFunction<void> Callback = m_scriptCallbackFunction;
	Callback(p->GetEventData());
	
}

bool InternalScriptExports::Init(void)
{
	s_pScriptEventListenerMgr = new ScriptEventListenerManager;
	return false;
}

void InternalScriptExports::Destroy(void)
{
	delete s_pScriptEventListenerMgr;
	s_pScriptEventListenerMgr = nullptr;
}

bool InternalScriptExports::LoadAndExecuteScriptResource(const char * scriptResource)
{
	gLuaState()->VExecuteFile(scriptResource);

	return false;
}

int InternalScriptExports::CreateActor(const char * actorArchetype, LuaPlus::LuaObject luaPosition, LuaPlus::LuaObject luaYawPitchRoll)
{
	if (!luaPosition.IsTable())
	{
		E_ERROR("Invalid object passed to CreateActor(); type = " + std::string(luaPosition.TypeName()));
		return 0;
	}

	if (!luaYawPitchRoll.IsTable())
	{
		E_ERROR("Invalid object passed to CreateActor(); type = " + std::string(luaYawPitchRoll.TypeName()));
		return 0;
	}

	vec3 pos(luaPosition["x"].GetFloat(), luaPosition["y"].GetFloat(), luaPosition["z"].GetFloat());
	vec3 ypr(luaYawPitchRoll["x"].GetFloat(), luaYawPitchRoll["y"].GetFloat(), luaYawPitchRoll["z"].GetFloat());

	mat4 initialTransform;
	mat4 translation;
	translation = glm::translate(translation, pos);

	mat4 rotation = glm::eulerAngleYXZ(ypr.y, ypr.x, ypr.z);

	initialTransform = translation*rotation;

	tinyxml2::XMLElement *overloads = NULL;
	Actor* pActor = gActorFactory()->CreateActor(actorArchetype, overloads, &initialTransform);

	if (pActor)
	{
		EvtData_New_Actor* pNewActorEvent(new EvtData_New_Actor(pActor->GetId()));
		gEventManager()->VQueueEvent(pNewActorEvent);
		return pActor->GetId();
	}

	return 0;
}

unsigned long InternalScriptExports::RegisterEventListener(EventType eventType, LuaPlus::LuaObject callbackFunction)
{
	assert(s_pScriptEventListenerMgr);

	if (callbackFunction.IsFunction())
	{
		// create the C++ listener proxy and set it to listen for the event
		ScriptEventListener* pListener = new ScriptEventListener(eventType, callbackFunction);
		s_pScriptEventListenerMgr->AddListener(pListener);
		gEventManager()->VAddListener(pListener->GetDelegate(), eventType);

		// convert the pointer to an unsigned long to use as the handle
		unsigned long handle = reinterpret_cast<unsigned long>(pListener);
		return handle;
	}

	E_ERROR("Attempting to register script event listener with invalid callback function");
	return 0;
}

void InternalScriptExports::RemoveEventListener(unsigned long listenerId)
{
	assert(s_pScriptEventListenerMgr);
	assert(listenerId != 0);
	// convert the listenerId back into a pointer
	ScriptEventListener* pListener = reinterpret_cast<ScriptEventListener*>(listenerId);
	s_pScriptEventListenerMgr->DestroyListener(pListener);  // the destructor will remove the listener
}

bool InternalScriptExports::QueueEvent(EventType eventType, LuaPlus::LuaObject eventData)
{
	ScriptEvent* pEvent(BuildEvent(eventType, eventData));
	if (pEvent)
	{
		gEventManager()->VQueueEvent(pEvent);
		return true;

	}
	return false;
}

bool InternalScriptExports::TriggerEvent(EventType eventType, LuaPlus::LuaObject eventData)
{
	ScriptEvent* pEvent(BuildEvent(eventType, eventData));
	if (pEvent)
	{
		gEventManager()->VTriggerEvent(pEvent);
		return true;
	}
	return false;
}

void InternalScriptExports::AttachScriptProcess(LuaPlus::LuaObject scriptProcess)
{
	E_ERROR("This code doesn't have anything.");
}

void InternalScriptExports::LuaLog(LuaPlus::LuaObject text)
{
	if (text.IsConvertibleToString())
	{
		E_DEBUG((string("Lua ") + string(text.ToString())));
	}
	else
	{
		E_DEBUG(string("Lua <") +  std::string(text.TypeName()) + string(">"));
	}
}

unsigned long InternalScriptExports::GetTickCount(void)
{
	return ::GetTickCount();
}

void InternalScriptExports::ApplyForce(LuaPlus::LuaObject normalDir, float force, int actorId)
{
	if (normalDir.IsTable())
	{
		vec3 normalDir(normalDir["x"].GetFloat(), normalDir["y"].GetFloat(), normalDir["z"].GetFloat());
		gPhysic()->VApplyForce(normalDir, force, actorId);
		return;
	}
	E_ERROR(string("Invalid object passed to ApplyForce(); type = ") + std::string(normalDir.TypeName()));
}

void InternalScriptExports::ApplyTorque(LuaPlus::LuaObject axis, float force, int actorId)
{
	if (axis.IsTable())
	{
		vec3 normalDir(axis["x"].GetFloat(), axis["y"].GetFloat(), axis["z"].GetFloat());
		gPhysic()->VApplyTorque(normalDir, force, actorId);
		return;
	}
	E_ERROR(string("Invalid object passed to ApplyForce(); type = ") + std::string(axis.TypeName()));
}

ScriptEvent * InternalScriptExports::BuildEvent(EventType eventType, LuaPlus::LuaObject & eventData)
{
	// create the event from the event type
	ScriptEvent* pEvent(ScriptEvent::CreateEventFromScript(eventType));
	if (!pEvent)
		return nullptr;

	// set the event data that was passed in
	if (!pEvent->SetEventData(eventData))
		return nullptr;

	return pEvent;
}

void ScriptExports::Register(void)
{
	LuaPlus::LuaObject globals = gLuaState()->GetGlobalVars();

	// Script Component Register
	ScriptComponent::RegisterScriptFunctions();
	// init	
	InternalScriptExports::Init();

	// resource loading
	globals.RegisterDirect("LoadAndExecuteScriptResource", &InternalScriptExports::LoadAndExecuteScriptResource);

	// actors
	globals.RegisterDirect("CreateActor", &InternalScriptExports::CreateActor);

	// event system
	globals.RegisterDirect("RegisterEventListener", &InternalScriptExports::RegisterEventListener);
	globals.RegisterDirect("RemoveEventListener", &InternalScriptExports::RemoveEventListener);
	globals.RegisterDirect("QueueEvent", &InternalScriptExports::QueueEvent);
	globals.RegisterDirect("TriggerEvent", &InternalScriptExports::TriggerEvent);

	// process system
	//globals.RegisterDirect("AttachProcess", &InternalScriptExports::AttachScriptProcess);

	// math
	//LuaPlus::LuaObject mathTable = globals.GetByName("GccMath");
	//GCC_ASSERT(mathTable.IsTable());
	//mathTable.RegisterDirect("GetYRotationFromVector", &InternalScriptExports::GetYRotationFromVector);
	//mathTable.RegisterDirect("WrapPi", &InternalScriptExports::WrapPi);
	//mathTable.RegisterDirect("GetVectorFromRotation", &InternalScriptExports::GetVectorFromRotation);

	// misc
	globals.RegisterDirect("Log", &InternalScriptExports::LuaLog);
	globals.RegisterDirect("GetTickCount", &InternalScriptExports::GetTickCount);

	// Physics
	globals.RegisterDirect("ApplyForce", &InternalScriptExports::ApplyForce);
	globals.RegisterDirect("ApplyTorque", &InternalScriptExports::ApplyTorque);


	// Timer
	LuaPlus::LuaObject TimerTable = globals.CreateTable("Timer");
	TimerTable.SetLightUserData("__object", gTimer());
	TimerTable.RegisterObjectDirect("GetCurrentTime", gTimer(), &GameTimer::GetGameTime);
	TimerTable.RegisterObjectDirect("GetDeltaTime", gTimer(), &GameTimer::GetDeltaTime);
	TimerTable.RegisterObjectDirect("GetFPS", gTimer(), &GameTimer::GetFPS);

	// Input
	LuaPlus::LuaObject InputTable = globals.CreateTable("Input");
	InputTable.SetLightUserData("__object", gInput());
	InputTable.RegisterObjectDirect("KeyDown", gInput(), &DirectInput::KeyDown);
	InputTable.RegisterObjectDirect("MouseButtonDown", gInput(), &DirectInput::MouseButtonDown);
	InputTable.RegisterObjectDirect("mouseDX", gInput(), &DirectInput::mouseDX);
	InputTable.RegisterObjectDirect("mouseDY", gInput(), &DirectInput::mouseDY);
	
}

void ScriptExports::Unregister(void)
{
	InternalScriptExports::Destroy();
}

#pragma once
#include "pch.h"

#define MAX_NAME 128

typedef unsigned int ActorId;
typedef unsigned int ComponentId;

class Scene;
class Actor;
class SceneNode;
class Mesh;
class Windows;
class ScriptEvent;
class Debug;
struct Texture;


class Material
{
public:
	
	vec3 Ka;
	vec3 Kd;
	vec3 Ks;
	vec3 exp;
	Material() :Ka(1.0f), Kd(1.0f), Ks(1.0f), exp(128)
	{

	}
};

// mesh interface
class IMesh
{
public:
	string					Name;
	unsigned int			VAO;
	unsigned int			VBO;
	unsigned int			EBO;
	unsigned int			NumIndices;
	Texture*				Tex;
	vec3					Color;
	GLuint					Topology;
	Material				material;
	// Use to generate Vertex Buffer Object, Vertex Array Object.
	virtual void Finalize(Shader* p) = 0;
	virtual void Scale(vec3 scale) {};

	
	// Virtual destructor
	virtual ~IMesh() {};
};

class ISceneNode
{
public:

	virtual void VSetName(string name) = 0;
	virtual string VGetName() = 0;
	virtual void VSetTransform(const mat4 *toWorld) = 0;

	// return transform with parent
	virtual mat4 VGetTransform() = 0;

	// return Global transform 
	virtual mat4 VGetGlobalTransform() = 0;

	// Update Scene Node
	virtual HRESULT VOnUpdate(Scene *pScene, float elapsedMs) = 0;

	virtual ActorId GetId(void) const = 0;
	virtual HRESULT VPreRender(Scene *pScene) = 0;
	virtual bool VIsVisible(Scene *pScene) const = 0;
	virtual HRESULT VRender(Scene *pScene) = 0;
	virtual HRESULT VRenderChildren(Scene *pScene) = 0;
	virtual HRESULT VPostRender(Scene *pScene) = 0;

	virtual bool VAddChild(std::unique_ptr<Actor> kid) = 0;
	virtual bool VRemoveChild(ActorId id) =0;
	virtual Actor* VGetParent() = 0;
	

	//virtual HRESULT VOnLostDevice(Scene *pScene) = 0;
	//virtual HRESULT VPick(Scene *pScene, RayCast *pRayCast) = 0;

	virtual ~ISceneNode() { };
};


/////////////////////////////////////////////////////////////////////////////
// class IGameWorld							
//
//   The interface defintion for a generic World map
/////////////////////////////////////////////////////////////////////////////

class IGameWorld
{
public:
	//virtual void VInitialize() = 0;
	//virtual void VUpdate(float dt) = 0;
	//virtual void VRender() = 0;
	virtual ~IGameWorld() {};
};
/////////////////////////////////////////////////////////////////////////////
// class IGamePhysics							
//
//   The interface defintion for a generic physics API.
/////////////////////////////////////////////////////////////////////////////

class IGamePhysics
{
public:

	// Initialiazation and Maintenance of the Physics World
	virtual bool VInitialize() = 0;
	virtual void VSyncVisibleScene() = 0;
	virtual void VOnUpdate(float deltaSeconds) = 0;
	virtual void VPreStep(float timeStep) = 0;
	virtual void VPostStep(float timeStep) = 0;

	virtual void VRemoveActor(ActorId id) = 0;
	// Debugging
	virtual void VRenderDiagnostics() = 0;

	virtual ~IGamePhysics() { };
};


class IEvent;
typedef unsigned long EventType;
typedef fastdelegate::FastDelegate1<std::shared_ptr<const IEvent>> EventListenerDelegate;
//typedef concurrent_queue<IEvent*> ThreadSafeEventQueue;



class IEvent
{
public:
	virtual ~IEvent(void) {}
	virtual const EventType& VGetEventType(void) const = 0;
	virtual float GetTimeStamp(void) const = 0;
	virtual void VSerialize(std::ostrstream& out) const = 0;
	virtual void VDeserialize(std::istrstream& in) = 0;
	virtual IEvent* VCopy(void) const = 0;
	virtual const char* GetName(void) const = 0;

};

class BaseEventData : public IEvent
{
	const float m_timeStamp;

public:
	explicit BaseEventData(const float timeStamp = 0.0f) : m_timeStamp(timeStamp) { }

	// Returns the type of the event
	virtual const EventType& VGetEventType(void) const = 0;

	float GetTimeStamp(void) const { return m_timeStamp; }

	// Serializing for network input / output
	virtual void VSerialize(std::ostrstream &out) const { }
	virtual void VDeserialize(std::istrstream& in) { }
};

class IEventManager
{
public:

	enum eConstants { kINFINITE = 0xffffffff };

	// Registers a delegate function that will get called when the event type is triggered.  Returns true if 
	// successful, false if not.
	virtual bool VAddListener(const EventListenerDelegate& eventDelegate, const EventType& type) = 0;

	// Removes a delegate / event type pairing from the internal tables.  Returns false if the pairing was not found.
	virtual bool VRemoveListener(const EventListenerDelegate& eventDelegate, const EventType& type) = 0;

	// Fire off event NOW.  This bypasses the queue entirely and immediately calls all delegate functions registered 
	// for the event.
	virtual bool VTriggerEvent(std::shared_ptr<const IEvent> pEvent) const = 0;

	// Fire off event.  This uses the queue and will call the delegate function on the next call to VTick(), assuming
	// there's enough time.
	virtual bool VQueueEvent(std::shared_ptr<const IEvent> pEvent) = 0;
	virtual bool VThreadSafeQueueEvent(std::shared_ptr<const IEvent> pEvent) = 0;

	// Find the next-available instance of the named event type and remove it from the processing queue.  This 
	// may be done up to the point that it is actively being processed ...  e.g.: is safe to happen during event
	// processing itself.
	//
	// if allOfType is true, then all events of that type are cleared from the input queue.
	//
	// returns true if the event was found and removed, false otherwise
	virtual bool VAbortEvent(const EventType& type, bool allOfType = false) = 0;

	// Allow for processing of any queued messages, optionally specify a processing time limit so that the event 
	// processing does not take too long. Note the danger of using this artificial limiter is that all messages 
	// may not in fact get processed.
	//
	// returns true if all messages ready for processing were completed, false otherwise (e.g. timeout )
	virtual bool VUpdate(unsigned long maxMillis = kINFINITE) = 0;

	virtual ~IEventManager(void) {};

};

class IScriptManager
{
public:
	virtual ~IScriptManager(void) {}
	virtual bool VInit(void) = 0;
	virtual void VExecuteFile(const char* resource) = 0;
	virtual void VExecuteString(const char* str) = 0;
};

class IApplication
{

public:
	virtual void Setup() {};
	virtual void Start() {};
	virtual void MainLoop() = 0;
	virtual ~IApplication() {};
};

class Context;
class ISubSystem
{
protected:
	
public:
	virtual void Init(Context* c) = 0;
	virtual void ShutDown() = 0;
	virtual ~ISubSystem() {};
	static Context* m_Context;
};


// Our game
class IGame
{
public:
	virtual void Init() {};
	virtual void Update(float dt) {};
	virtual void Render() {};
	virtual ~IGame() {};
};
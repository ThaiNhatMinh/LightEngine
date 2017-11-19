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


struct Material
{
	vec3 Ka;
	vec3 Kd;
	vec3 Ks;
	float exp;
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

	virtual Shader* VGetShader() = 0;
	virtual void VSetShader(Shader* p) = 0;
	virtual void VSetShader(const char* pName) = 0;
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

	virtual bool VAddChild(Actor* kid) = 0;
	virtual bool VRemoveChild(ActorId id) =0;
	virtual Actor* VGetParent() = 0;
	

	//virtual HRESULT VOnLostDevice(Scene *pScene) = 0;
	//virtual HRESULT VPick(Scene *pScene, RayCast *pRayCast) = 0;

	virtual ~ISceneNode() { };
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

	// Initialization of Physics Objects
	//virtual void VAddSphere(float radius, Actor* actor, /*const Mat4x4& initialTransform, */const std::string& densityStr, const std::string& physicsMaterial) = 0;
	//virtual void VAddBox(const vec3& dimensions, Actor* gameActor, /*const Mat4x4& initialTransform, */ const std::string& densityStr, const std::string& physicsMaterial) = 0;
	//virtual void VAddPointCloud(vec3 *verts, int numPoints, Actor* gameActor, /*const Mat4x4& initialTransform, */ const std::string& densityStr, const std::string& physicsMaterial) = 0;
	virtual void VRemoveActor(ActorId id) = 0;
	//virtual void VAddCharacter(const vec3& dimensions, Actor* gameActor) = 0;
	// Debugging
	virtual void VRenderDiagnostics() = 0;

	// Physics world modifiers
	//virtual void VCreateTrigger(Actor* pGameActor, const vec3 &pos, const float dim) = 0;
	//virtual void VApplyForce(const vec3 &dir, float newtons, ActorId aid) = 0;
	//virtual void VApplyTorque(const vec3 &dir, float newtons, ActorId aid) = 0;
	//virtual bool VKinematicMove(const mat4 &mat, ActorId aid) = 0;

	// Physics actor states
	//virtual void VRotateY(ActorId actorId, float angleRadians, float time) = 0;
	//virtual float VGetOrientationY(ActorId actorId) = 0;
	//virtual void VStopActor(ActorId actorId) = 0;
	//virtual vec3 VGetVelocity(ActorId actorId) = 0;
	//virtual void VSetVelocity(ActorId actorId, const vec3& vel) = 0;
	//virtual vec3 VGetAngularVelocity(ActorId actorId) = 0;
	//virtual void VSetAngularVelocity(ActorId actorId, const vec3& vel) = 0;
	//virtual void VTranslate(ActorId actorId, const vec3& vec) = 0;

	////virtual void VSetTransform(const ActorId id, const mat4& mat) = 0;
	//virtual mat4 VGetTransform(const ActorId id) = 0;

	//virtual void VClearForce(ActorId id) = 0;
	virtual ~IGamePhysics() { };
};


class IEvent;
typedef unsigned long EventType;
typedef fastdelegate::FastDelegate1<const IEvent*> EventListenerDelegate;
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
	virtual bool VTriggerEvent(const IEvent* pEvent) const = 0;

	// Fire off event.  This uses the queue and will call the delegate function on the next call to VTick(), assuming
	// there's enough time.
	virtual bool VQueueEvent(const IEvent* pEvent) = 0;
	virtual bool VThreadSafeQueueEvent(const IEvent* pEvent) = 0;

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
	virtual const Debug& GetDebug()=0;
	virtual void MainLoop() = 0;
	virtual ~IApplication() {};
};
extern IApplication* APP;
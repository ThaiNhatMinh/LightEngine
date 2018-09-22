#pragma once
#include <pch.h>

#define MAX_NAME 128

class Scene;


#include "Interface\ISubSystem.h"
#include "Interface\IContext.h"

#include "Interface\IWindow.h"
#include "Interface\IInput.h"
#include "Interface\IDebugRender.h"
#include "Interface\IResourceManager.h"

//#include "Interface\ISysUI.h"
//#include "Interface\ISoundEngine.h"

//#include "Interface\IActor.h"
//#include "Interface\IComponent.h"
//#include "Interface\IFactory.h"
//#include "Interface\IShader.h"
//#include "Interface\IEvent.h"
//#include "Interface\IEventManager.h"
//#include "Interface\IResource.h"
//#include "Interface\IModelResource.h"
//#include "Interface\IResourceManager.h"


/*class ISceneNode
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
};*/
/////////////////////////////////////////////////////////////////////////////
// class IGamePhysics							
//
//   The interface defintion for a generic physics API.
/////////////////////////////////////////////////////////////////////////////


//
//class IEvent;
//
//
//#define EVENT_DEFINE(typeName) \
//	virtual const EventType& VGetEventType(void) const {return sk_EventType;};\
//	virtual void VSerialize(std::ostrstream& out) const{};\
//	virtual void VDeserialize(std::istrstream& in){};\
//	virtual const char* GetName(void) const{return #typeName;};\
//	static const EventType sk_EventType;\
//
//class BaseEventData : public IEvent
//{
//	const float m_timeStamp;
//
//public:
//	explicit BaseEventData(const float timeStamp = 0.0f) : m_timeStamp(timeStamp) { }
//
//	// Returns the type of the event
//	virtual const EventType& VGetEventType(void) const = 0;
//
//	float GetTimeStamp(void) const { return m_timeStamp; }
//
//	// Serializing for network input / output
//	virtual void VSerialize(std::ostrstream &out) const { }
//	virtual void VDeserialize(std::istrstream& in) { }
//};

class Context;


// Our game
class IGame
{
public:
	virtual void	Init(Context*) {};
	virtual void	Update(float dt) {};
	virtual void	Render() {};
	virtual void	ShutDown() {};
	virtual Scene*	GetScene() = 0;
	virtual			~IGame() {};
};
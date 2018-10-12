#pragma once


class Context;
//class Scene;

class IGamePlugin
{
public:
	// Call after engine init
	virtual void	Init(Light::IContext*) =0;
	// Call every frame
	virtual void	Update(float dt) = 0;
	// Call before engine shutdown
	virtual void	ShutDown() = 0;
	// Get Scene in game
	//virtual Scene*	GetScene() = 0;
	virtual ~IGamePlugin() {};
};


#define CREATEINTERFACE_PROCNAME	"CreateInterface"
typedef IGamePlugin* (*CreateInterfaceFn)();

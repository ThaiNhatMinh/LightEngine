#pragma once

class Context;
class Scene;

class IGamePlugin
{
public:
	// Call after engine init
	virtual void	Init(Context*) =0;
	// Call every frame
	virtual void	Update(float dt) = 0;
	// Call every frame
	virtual void	Render() = 0;
	// Call before engine shutdown
	virtual void	ShutDown() = 0;
	// Get Scene in game
	virtual Scene*	GetScene() = 0;
	virtual ~IGamePlugin() {};
};


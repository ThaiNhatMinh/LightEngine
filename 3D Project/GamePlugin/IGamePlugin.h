#pragma once

#include "IContextDLL.h"

class IGamePlugin
{
public:
	// Call after engine init
	virtual void	Init(IContextDLL*) =0;
	// Call every frame
	virtual void	Update(float dt) = 0;
	// Call every frame
	virtual void	Render() = 0;
	// Call before engine shutdown
	virtual void	ShutDown() = 0;

	virtual ~IGamePlugin() {};
};


#include "pch.h"

void main()
{

#if defined(DEBUG) || defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	CoreApplication::startUp();

	//ActorFactory fac;
	//mat4 ddd;
	//Actor* pAc = fac.CreateActor("player_teapot.xml", nullptr, &ddd);
	
	CoreApplication::InstancePtr()->MainLoop();
	CoreApplication::shutDown();
}
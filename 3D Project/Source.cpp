#include "pch.h"

void main()
{
	
#if defined(DEBUG) || defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	

	char pwd[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, pwd);
	std::string str(pwd);

	File::SetExecDir(str.c_str());

	CoreApplication::startUp();
	
	CoreApplication::InstancePtr()->MainLoop();
	CoreApplication::shutDown();

}
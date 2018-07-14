

#include <pch.h>
#include "Core\Application.h"


class Test { virtual void CAC() {} };
class B {};
class C : public B, public Test {};

void main()
{

#if defined(DEBUG) || defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);
#endif


	Application app;
	app.MainLoop();
	
	
}


#include <pch.h>
#include "Core\Application.h"

#include <thread>
#include "Interface\IEvent.h"
#include "Utilities\Delegate.h"
class A
{
public:
	void foo(std::shared_ptr<Light::IEvent> )
	{
		std::cout << "A: "  <<std::endl;
	}
};
class B
{
public:
	void foo1(std::shared_ptr<Light::IEvent>)
	{
		std::cout << "B" << std::endl;
	}
};
void main()
{

#if defined(DEBUG) || defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);
#endif

	unsigned int n = std::thread::hardware_concurrency();
	std::cout << n << " concurrent threads are supported.\n";
	Application app;
	app.MainLoop();
	
	system("pause");
}
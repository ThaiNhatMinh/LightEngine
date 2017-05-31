#include "..\pch.h"


void CoreApplication::onStartUp()
{
	E_DEBUG("Application StartUp...");
	m_pWindow = new Windows("Ex-T1", 800, 600);
	m_pWindow->InitWindow();
	Input::startUp();
	GameTimer::startUp();
	Resources::startUp();
}

void CoreApplication::onShutDown()
{
	E_DEBUG("Application ShutDown...");
	Input::shutDown();
	GameTimer::shutDown();
	Resources::shutDown();
	delete m_pWindow;

	
}

bool CoreApplication::MainLoop()
{
	m_bRunMainLoop = true;
	gTimer()->Reset();
	while (m_bRunMainLoop)
	{
		glfwPollEvents();
		if (glfwWindowShouldClose(m_pWindow->Window())) m_bRunMainLoop = false;

		gTimer()->Tick();

		if (gInput()->Press(GLFW_KEY_ESCAPE)) glfwSetWindowShouldClose(m_pWindow->Window(), GLFW_TRUE);

		glfwSwapBuffers(m_pWindow->Window());
	}

	return false;
}

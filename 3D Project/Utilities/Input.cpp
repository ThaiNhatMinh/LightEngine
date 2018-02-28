#include "pch.h"


/*Input* gInput()
{
	return Input::InstancePtr();
}*/
Input::Input(Context* c):ISubSystem(c)
{
	memset(keys, 0, 1024);
	memset(checkKeys, 0, 1024);
	c->AddSystem(this);
}

Input::~Input()
{
}

bool Input::Press(int key)
{
	if (keys[key] && checkKeys[key])
	{
		checkKeys[key] = false;
		return true;
	}

	return false;
}

bool Input::MousePress(int m)
{
	if (Mouse[m] && checkMouse[m])
	{
		checkMouse[m] = false;
		return true;
	}

	return false;
}

/*
// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key < 0) return;
	//if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	//	glfwSetWindowShouldClose(window, GL_TRUE);

	//if (key == GLFW_KEY_F && action == GLFW_PRESS)
	//	gWindow.SwitchMode();
	//if (key == GLFW_KEY_T && action == GLFW_PRESS)
	//	bUpdateF = !bUpdateF;
	if (action == GLFW_PRESS)
	{
		gInput()->checkKeys[key] = true;
		gInput()->keys[key] = true;
	}
	else if (action == GLFW_RELEASE)
	{
		gInput()->checkKeys[key] = false;
		gInput()->keys[key] = false;
	}
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (action == GLFW_PRESS)
	{
		gInput()->checkMouse[button] = true;
		gInput()->Mouse[button] = true;
	}
	else if (action == GLFW_RELEASE)
	{
		gInput()->checkMouse[button] = false;
		gInput()->Mouse[button] = false;
	}
}
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	static bool firstMouse = false;
	static double last_x = 0;
	static double last_y = 0;

	if (!firstMouse)
	{
		last_x = xpos;
		last_y = ypos;
		firstMouse = true;
	}

	float dx = (float)(xpos - last_x);
	float dy = -(float)(ypos - last_y);
	gInput()->MouseD[0] = dx;
	gInput()->MouseD[1] = dy;
	last_x = xpos;
	last_y = ypos;
	
}
void scroll_callback(GLFWwindow*, double x, double y)
{
	//cout << x << y << endl;
	//mNear += y;
	//Projection = glm::Perspective(30.0f, 4.0 / 3.0, mNear, 500.0f);
	//frustum.Init(30.0f, 4.0f / 3.0f, mNear, 500.0f);
}*/
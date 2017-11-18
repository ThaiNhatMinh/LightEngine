#pragma once

#include "pch.h"

//void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

class Input : public Singleton<Input>
{
private:
	bool keys[1024]; // hold key status
	bool checkKeys[1024]; // hold last keu status
	bool Mouse[3];
	bool checkMouse[3];
	float MouseD[2];
public:
	Input();
	~Input();

	bool OnKey(int Key) {return keys[Key];	};
	bool Press(int key);
	bool OnMouse(int m) { return Mouse[m]; };
	bool MousePress(int m);
	float dX() { return MouseD[0]; }
	float dY() { return MouseD[1]; }
	friend void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
	friend void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
	friend void mouse_callback(GLFWwindow* window, double xpos, double ypos);
};

//Input* gInput();
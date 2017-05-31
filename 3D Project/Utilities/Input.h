#pragma once

#include "..\pch.h"

//void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

class Input : public Singleton<Input>
{
private:
	bool keys[1024]; // hold key status
	bool checkKeys[1024]; // hold last keu status
	bool Mouse[3];
	bool checkMouse[3];
public:
	Input();
	~Input();

	bool OnKey(int Key) {return keys[Key];	};
	bool Press(int key);
	bool OnMouse(int m) { return Mouse[m]; };
	bool MousePress(int m);
	friend void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
	friend void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
};

Input* gInput();
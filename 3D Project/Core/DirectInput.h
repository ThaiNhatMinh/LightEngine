#pragma once

#include "pch.h"

class DirectInput: public ISubSystem
{
private:
	IDirectInput8*				m_pDInput;
	IDirectInputDevice8*		m_pKeyboard;
	char						m_KeyState[256];
	IDirectInputDevice8*		m_pMouse;
	DIMOUSESTATE2				m_MouseState;
	
public:
	DirectInput();
	~DirectInput();

	void Init(Context* c);
	void ShutDown();
	void Update();
	bool KeyDown(char key);
	bool MouseButtonDown(int button);
	float mouseDX();
	float mouseDY();
	float mouseDZ();

private:
	DirectInput(const DirectInput& other);
	DirectInput& operator=(const DirectInput& other);

};

#pragma once

#include "..\pch.h"

class DirectInput: public Singleton<DirectInput>
{
private:
	IDirectInput8*				m_pDInput;
	IDirectInputDevice8*		m_pKeyboard;
	char						m_KeyState[256];
	IDirectInputDevice8*		m_pMouse;
	DIMOUSESTATE2				m_MouseState;
	bool						hasInit;
public:
	DirectInput(Windows* p,DWORD keyboardCoopFlags, DWORD mouseCoopFlag);
	//DirectInput();
	~DirectInput();

	//void Init(BaseD3D* pApp,DWORD keyboardCoopFlags, DWORD mouseCoopFlag);
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

//extern DirectInput* gDInput;
DirectInput* gInput();
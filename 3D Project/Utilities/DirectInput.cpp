#include "pch.h"

//DirectInput* gDInput;
DirectInput* gInput()
{
	return DirectInput::InstancePtr();
}

DirectInput::DirectInput(Windows* pWin,DWORD keyboardCoopFlags, DWORD mouseCoopFlag):hasInit(0)
{
	//Init(pApp, keyboardCoopFlags, mouseCoopFlag);
	if (hasInit) return;

	ZeroMemory(m_KeyState, sizeof(m_KeyState));
	ZeroMemory(&m_MouseState, sizeof(m_MouseState));

	HRESULT hr = DirectInput8Create(GetModuleHandle(NULL), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pDInput, 0);
	if (FAILED(hr))
	{

	}

	HWND handle = GetActiveWindow();
	m_pDInput->CreateDevice(GUID_SysKeyboard, &m_pKeyboard, 0);
	m_pKeyboard->SetDataFormat(&c_dfDIKeyboard);
	m_pKeyboard->SetCooperativeLevel(pWin->GetHandle(), keyboardCoopFlags);
	m_pKeyboard->Acquire();

	m_pDInput->CreateDevice(GUID_SysMouse, &m_pMouse, 0);
	m_pMouse->SetDataFormat(&c_dfDIMouse2);
	m_pMouse->SetCooperativeLevel(pWin->GetHandle(), mouseCoopFlag);
	m_pMouse->Acquire();

	hasInit = 1;
}

/*
DirectInput::DirectInput() :m_pDInput(0), m_pKeyboard(0), m_pMouse(0), hasInit(0)
{
}
*/
DirectInput::~DirectInput()
{
	m_pDInput->Release();
	m_pKeyboard->Unacquire();
	m_pKeyboard->Release();
	m_pMouse->Unacquire();
	m_pMouse->Release();

}

void DirectInput::Update()
{
	HRESULT hr = m_pKeyboard->GetDeviceState(sizeof(m_KeyState), (void**)&m_KeyState);
	if (FAILED(hr))
	{
		// Keyboard lost, zero out keyboard data structure.
		ZeroMemory(m_KeyState, sizeof(m_KeyState));

		// Try to acquire for next time we poll.
		hr = m_pKeyboard->Acquire();
	}

	hr = m_pMouse->GetDeviceState(sizeof(DIMOUSESTATE2), (void**)&m_MouseState);
	if (FAILED(hr))
	{
		// Keyboard lost, zero out keyboard data structure.
		ZeroMemory(&m_MouseState, sizeof(m_MouseState));

		// Try to acquire for next time we poll.
		hr = m_pMouse->Acquire();
	}
}

bool DirectInput::KeyDown(char key)
{
	return (m_KeyState[key] & 0x80) != 0;
}

bool DirectInput::MouseButtonDown(int button)
{
	return  (m_MouseState.rgbButtons[button] & 0x80) != 0;;
}

float DirectInput::mouseDX()
{
	return (float)m_MouseState.lX;
}

float DirectInput::mouseDY()
{
	return (float)-m_MouseState.lY;
}

float DirectInput::mouseDZ()
{
	return (float)m_MouseState.lZ;
}

DirectInput & DirectInput::operator=(const DirectInput & other)
{
	// TODO: insert return statement here
	return *this;
}

/************************************************************************/
/* Author:  Kemi Peng 
History: 08/03/2015 by Kemi Peng*/
/************************************************************************/

#include "StandardInput.h"
#include "Window_Win32.h"
StandardInput::StandardInput( void )
{
	
}


void StandardInput::initiate( void )
{
#ifdef WIN32
	ZeroMemory(m_keyboardState, sizeof(m_keyboardState));
	ZeroMemory(&m_mouseState, sizeof(m_mouseState));

	FAILED(DirectInput8Create(Window_Win32::GetEngineWindow()->getWindowInstanceWin32(), DIRECTINPUT_VERSION, 
		IID_IDirectInput8, (void**)&m_dInput, 0)) ;

	m_dInput->CreateDevice(GUID_SysKeyboard, &m_keyboard, 0);
	m_keyboard->SetDataFormat(&c_dfDIKeyboard);
	m_keyboard->SetCooperativeLevel(Window_Win32::GetEngineWindow()->getWindowHandleWin32(), DISCL_NONEXCLUSIVE|DISCL_FOREGROUND);
	m_keyboard->Acquire();

	m_dInput->CreateDevice(GUID_SysMouse, &m_mouse, 0);
	m_mouse->SetDataFormat(&c_dfDIMouse2);
	m_mouse->SetCooperativeLevel(Window_Win32::GetEngineWindow()->getWindowHandleWin32(), DISCL_NONEXCLUSIVE|DISCL_FOREGROUND);
	m_mouse->Acquire();
#endif
}

StandardInput::~StandardInput( void )
{
#ifdef WIN32
	SAFE_RELEASE(m_dInput);
	m_keyboard->Unacquire();
	m_mouse->Unacquire();
	SAFE_RELEASE(m_keyboard);
	SAFE_RELEASE(m_mouse);
#endif
}

bool StandardInput::isKey( unsigned char key ) const
{
#ifdef WIN32
	return (m_keyboardState[key] & 0x80) != 0;
#endif
}

void StandardInput::update( void )
{
#ifdef WIN32
	HRESULT hr = m_keyboard->GetDeviceState(sizeof(m_keyboardState), (void**)&m_keyboardState); 
	if( FAILED(hr) )
	{
		// Keyboard lost, zero out keyboard data structure.
		ZeroMemory(m_keyboardState, sizeof(m_keyboardState));

		// Try to acquire for next time we poll.
		hr = m_keyboard->Acquire();
	}

	// Poll mouse.
	hr = m_mouse->GetDeviceState(sizeof(DIMOUSESTATE2), (void**)&m_mouseState); 
	if( FAILED(hr) )
	{
		// Mouse lost, zero out mouse data structure.
		ZeroMemory(&m_mouseState, sizeof(m_mouseState));

		// Try to acquire for next time we poll.
		hr = m_mouse->Acquire(); 
	}
#endif
}

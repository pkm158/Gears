/************************************************************************/
/* Author:  Kemi Peng 
History: 08/03/2015 by Kemi Peng*/
/************************************************************************/

#ifndef STANDARDINPUT
#define STANDARDINPUT

#include "GearsCommonHead.h"
#ifdef WIN32
#include <dinput.h>
#endif

//slandered keyboard andcursor input
class StandardInput
{
public:
	StandardInput(void);
	~StandardInput(void);

	void	initiate(void);
	bool isKey(unsigned char key) const;	
	inline bool isMouseButton(int button) const { return (m_mouseState.rgbButtons[button] & 0x80) != 0; };
	inline float mouseDX() const { return (float)m_mouseState.lX; };
	inline float mouseDY() const { return (float)m_mouseState.lY; };
	inline float mouseDZ() const { return (float)m_mouseState.lZ; };

	void update(void);
	
private:
	StandardInput(const StandardInput& rhs);
	StandardInput& operator=(const StandardInput& rhs);

private:

	DIMOUSESTATE2        m_mouseState;
	IDirectInputDevice8* m_mouse;

	IDirectInput8*       m_dInput;
	IDirectInputDevice8* m_keyboard;
	char                 m_keyboardState[256]; 

	
};

#endif
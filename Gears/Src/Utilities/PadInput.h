#ifndef PADINPUT
#define PADINPUT

#include "GearsCommonHead.h"
#include "libmath.h"

#ifdef WIN32
#include <XInput.h> 
#endif

#define INPUT_DEADZONE  ( 0.24f * FLOAT(0x7FFF) )

struct CONTROLLER_STATE
{
	XINPUT_STATE lastState;
	XINPUT_STATE state;
	int dwResult;
	bool bLockVibration;
	XINPUT_VIBRATION vibration;

	bool bConnected;
};

struct CONTROLLER
{
	CVector RThumbStick;
	CVector	LThumbStick;

	unsigned char	leftTrigger,rightTrigger;

	bool	A;
	bool	X;
	bool	Y;
	bool	B;
	bool	LB;
	bool	RB;
	bool	BACK;
	bool	START;
	bool	L3;
	bool	R3;
	bool	UP;
	bool	LEFT;
	bool	DOWN;
	bool	RIGHT;

};

class PadInput
{
public:
	PadInput( void );
	~PadInput( void );
	bool	intController(void);

	void	update(void);


	void	leftMotor(unsigned int speed);
	void	rightMotor(unsigned int speed);

	inline bool	getConnectState() const { return m_conState.bConnected; };

	const CONTROLLER& getController(void) const { return m_xController; };

private:
	CONTROLLER_STATE m_conState;
	CONTROLLER	m_xController;
	void	tryConnect(void);
};

#endif
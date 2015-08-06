#include "PadInput.h"
PadInput::PadInput( void )
{
	memset(&m_conState, 0x00, sizeof(CONTROLLER_STATE));
	memset(&m_xController, 0x00, sizeof(CONTROLLER));
}

PadInput::~PadInput( void )
{

}

void PadInput::tryConnect( void )
{
	m_conState.lastState = m_conState.state;
	m_conState.dwResult = XInputGetState( 0, &m_conState.state );

	if( m_conState.dwResult == ERROR_SUCCESS )
	{
		m_conState.bConnected = true;
	}
	else
	{
		m_conState.bConnected = false;
		memset(&m_conState, 0x00, sizeof( CONTROLLER_STATE));
	}
}

void PadInput::update( void )
{
	tryConnect();

	if (m_conState.bConnected)
	{
		unsigned short wButtons = m_conState.state.Gamepad.wButtons;

		//这里有个盲区（DEADZONE）
		if( ( m_conState.state.Gamepad.sThumbLX < INPUT_DEADZONE &&
			m_conState.state.Gamepad.sThumbLX > -INPUT_DEADZONE ) &&
			( m_conState.state.Gamepad.sThumbLY < INPUT_DEADZONE &&
			m_conState.state.Gamepad.sThumbLY > -INPUT_DEADZONE ) )
		{
			m_conState.state.Gamepad.sThumbLX = 0;
			m_conState.state.Gamepad.sThumbLY = 0;
		}

		if( ( m_conState.state.Gamepad.sThumbRX < INPUT_DEADZONE &&
			m_conState.state.Gamepad.sThumbRX > -INPUT_DEADZONE ) &&
			( m_conState.state.Gamepad.sThumbRY < INPUT_DEADZONE &&
			m_conState.state.Gamepad.sThumbRY > -INPUT_DEADZONE ) )
		{
			m_conState.state.Gamepad.sThumbRX = 0;
			m_conState.state.Gamepad.sThumbRY = 0;
		}

		if (wButtons & XINPUT_GAMEPAD_A)
		{
			m_xController.A = true;
		}else
		{
			m_xController.A = false;
		}
		if (wButtons & XINPUT_GAMEPAD_X)
		{
			m_xController.X = true;
		}else
		{
			m_xController.X = false;
		}
		if (wButtons & XINPUT_GAMEPAD_Y)
		{
			m_xController.Y = true;
		}else
		{
			m_xController.Y = false;
		}
		if (wButtons & XINPUT_GAMEPAD_B)
		{
			m_xController.B = true;
		}else
		{
			m_xController.B = false;
		}
		if (wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER)
		{
			m_xController.LB = true;
		}else
		{
			m_xController.LB = false;
		}
		if (wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER)
		{
			m_xController.RB = true;
		}else
		{
			m_xController.RB = false;
		}
		if (wButtons & XINPUT_GAMEPAD_BACK)
		{
			m_xController.BACK = true;
		}else
		{
			m_xController.BACK = false;
		}
		if (wButtons & XINPUT_GAMEPAD_START)
		{
			m_xController.START = true;
		}else
		{
			m_xController.START = false;
		}
		if (wButtons & XINPUT_GAMEPAD_LEFT_THUMB)
		{
			m_xController.L3 = true;
		}else
		{
			m_xController.L3 = false;
		}
		if (wButtons & XINPUT_GAMEPAD_RIGHT_THUMB)
		{
			m_xController.R3 = true;
		}else
		{
			m_xController.R3 = false;
		}
		if (wButtons & XINPUT_GAMEPAD_DPAD_UP)
		{
			m_xController.UP = true;
		}else
		{
			m_xController.UP = false;
		}
		if (wButtons & XINPUT_GAMEPAD_DPAD_DOWN)
		{
			m_xController.DOWN = true;
		}else
		{
			m_xController.DOWN = false;
		}
		if (wButtons & XINPUT_GAMEPAD_DPAD_LEFT)
		{
			m_xController.LEFT = true;
		}else
		{
			m_xController.LEFT = false;
		}
		if (wButtons & XINPUT_GAMEPAD_DPAD_RIGHT)
		{
			m_xController.RIGHT = true;
		}else
		{
			m_xController.RIGHT = false;
		}

		m_xController.LThumbStick = CVector( (float)m_conState.state.Gamepad.sThumbLX, (float)m_conState.state.Gamepad.sThumbLY, 0, 0 );
		m_xController.RThumbStick = CVector( (float)m_conState.state.Gamepad.sThumbRX, (float)m_conState.state.Gamepad.sThumbRY, 0, 0 );
		m_xController.leftTrigger = m_conState.state.Gamepad.bLeftTrigger;
		m_xController.rightTrigger = m_conState.state.Gamepad.bRightTrigger;
	}
}

void PadInput::leftMotor( unsigned int speed )
{
	m_conState.vibration.wLeftMotorSpeed = speed;

	XInputSetState( 0, &m_conState.vibration );
}

void PadInput::rightMotor( unsigned int speed )
{
	m_conState.vibration.wRightMotorSpeed = speed;

	XInputSetState( 0, &m_conState.vibration );
}
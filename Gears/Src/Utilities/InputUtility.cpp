/************************************************************************/
/* Author:  Kemi Peng 
History: 08/03/2015 by Kemi Peng*/
/************************************************************************/
#include "InputUtility.h"

InputUtility::InputUtility( void )
{
	DEBUG_PRINT("Create input utility...");
	IUtility::m_UtilityType = UTILITY_INPUT;
}

InputUtility::~InputUtility( void )
{

}

void InputUtility::update( double tt, double dt )
{
	m_standardInput.update();
	m_padInput.update();

}

int InputUtility::getKeyState( KeyType key ) const
{
	switch (key)
	{
	case KUP:
		return m_standardInput.isKey(DIK_W)||m_padInput.getController().UP;
		break;

	case KLEFT:
		return m_standardInput.isKey(DIK_A)||m_padInput.getController().LEFT;
		break;

	case KDOWN:
		return m_standardInput.isKey(DIK_S)||m_padInput.getController().DOWN;
		break;

	case KRIGHT:
		return m_standardInput.isKey(DIK_D)||m_padInput.getController().RIGHT;
		break;
	default:
		return 0;
	}
}

void InputUtility::postInitiate( void )
{
	DEBUG_PRINT("Initiate input utility...");
	m_standardInput.initiate();
}

bool InputUtility::isKey( unsigned char key ) const
{
	return m_standardInput.isKey(key);
}
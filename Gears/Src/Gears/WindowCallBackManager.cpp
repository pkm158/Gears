/************************************************************************/
/* Author:  Kemi Peng 
History: 08/03/2015 by Kemi Peng*/
/************************************************************************/
#include "WindowCallBackManager.h"
#include "windows.h"
#include "Window_Win32.h"

WindowCallBackManager::~WindowCallBackManager( void )
{
	
}

void WindowCallBackManager::onResetDevice( void )
{
	for ( std::vector< IWindowCallBack* >::iterator ite = m_callBackVector.begin(); ite != m_callBackVector.end(); ++ite )
	{
		(*ite)->onLost();	
		(*ite)->onReset();
	}

	for ( std::vector< IReset* >::iterator ite = m_resetVector.begin(); ite != m_resetVector.end(); ++ite )
	{
		if ( !(*ite)->reset() )
		{
			ALERT( L"Reset Failed!" );
		}
	
	}
}

void WindowCallBackManager::addCallBack( IWindowCallBack* callBack )
{
	m_callBackVector.push_back( callBack );
}

void WindowCallBackManager::addReset( IReset* reset )
{
	m_resetVector.push_back( reset );
}
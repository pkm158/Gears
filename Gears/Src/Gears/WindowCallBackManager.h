/************************************************************************/
/* Author:  Kemi Peng 
History: 08/03/2015 by Kemi Peng*/
/************************************************************************/
#ifndef WINDOWCALLBACKMANAGER
#define WINDOWCALLBACKMANAGER

#include "GearsCommonHead.h"
#include "IWindowCallBack.h"
#include "IReset.h"
#include <vector>

class WindowCallBackManager
{
public:
	WindowCallBackManager( void ){};
	~WindowCallBackManager( void );

	void onResetDevice( void );
	void addCallBack( IWindowCallBack* callBack);

	void addReset( IReset* reset );

	std::vector <IWindowCallBack*> m_callBackVector;
	std::vector <IReset*> m_resetVector;
};

#endif
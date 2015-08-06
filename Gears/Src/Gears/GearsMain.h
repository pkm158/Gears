/************************************************************************/
/* Author:  Kemi Peng 
History: 08/03/2015 by Kemi Peng*/
/************************************************************************/
#ifndef GEARSMAIN
#define GEARSMAIN

#include "Window_Win32.h"
#include "WindowCallBackManager.h"
#include "RenderCore.h"
#include "UtilityManager.h"
class GearsMain
{
public:
	GearsMain(void);
	~GearsMain(void);

	bool creatWindow( void );
	void initDevice( void );
	void closeWindow( void );
	void run( void );

private:
	void update( double tt, double dt );
	WindowCallBackManager *m_pWinCallBack;
	RenderCore *m_pRenderCore;
};

#endif
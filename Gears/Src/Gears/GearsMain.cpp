/************************************************************************/
/* Author:  Kemi Peng 
History: 08/03/2015 by Kemi Peng*/
/************************************************************************/

#include "GearsMain.h"

#ifdef _DEBUG
// with console
void main()
{
	FILE	*fp = NULL;
	fopen_s( &fp, logFile, "wb");
	fclose(fp);



	UtilityManager::CreateUtilityManager();	//create and initiate every utility
	Window_Win32::CreateEngineWindow();
	GearsMain engine;
	HR( engine.creatWindow() );
	engine.initDevice();
	UtilityManager::GetHandle()->postInitiate();	//some utilities need windows or opengl information to complete its initiation, put them here.
	engine.run();
	Window_Win32::ReleaseEngineWindow();
	UtilityManager::ReleaseUtilityManager();
}
#else
// with out console
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
				   PSTR cmdLine, int showCmd)
{
	UtilityManager::CreateUtilityManager();
	Window_Win32::CreateEngineWindow();
	GearsMain engine;
	HR( engine.creatWindow() );
	engine.initDevice();
	UtilityManager::GetHandle()->postInitiate();	
	engine.run();
	Window_Win32::ReleaseEngineWindow();
	UtilityManager::ReleaseUtilityManager();
}
#endif

GearsMain::GearsMain(void):
				m_pWinCallBack( new WindowCallBackManager() ),
				m_pRenderCore( new RenderCore() )
{
}

GearsMain::~GearsMain(void)
{
	SAFE_DELETE( m_pRenderCore );
	SAFE_DELETE( m_pWinCallBack );
}

bool GearsMain::creatWindow( void )
{
	int w,h;

	GetWindowWH( w, h );
	const char * appName = GetAppName();

	int isfullScreen = IsFullScreen();

	if ( isfullScreen )
		return Window_Win32::GetEngineWindow()->fullScreen( w, h, appName );
	else
		return Window_Win32::GetEngineWindow()->createWindow( 50,50, w, h, appName );
}

void GearsMain::run( void )
{
	double dt = 0.001f;
	double tt = 0.f;
	int fps = GetFPS();
	TimerStart();
	do
	{
		if (IsKeyPressed(DIK_ESCAPE))
		{
			
			PostQuitMessage(0);
			DEBUG_PRINT("Quit by pressing esc...");
		}

		ClearRenderLists();

		do 
		{
			dt = GetTime();
		} while ( dt<(1.0f/fps) );	//lock fps
		TimerReset();
		TimerStart();

		tt += dt;
		update( tt,dt );

		m_pRenderCore->draw();

	} while( Window_Win32::GetEngineWindow()->processMessage() );
}

void GearsMain::closeWindow( void )
{
	Window_Win32::GetEngineWindow()->closeWindow();
}

void GearsMain::initDevice( void )
{
	m_pRenderCore->initDevice( m_pWinCallBack );
}

//tt, amount of time since game started. dt, amount of time of this frame.
void GearsMain::update( double tt, double dt )
{
	UtilityManager::GetHandle()->update( tt, dt );
}
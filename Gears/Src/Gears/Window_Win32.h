/************************************************************************/
/* Author:  Kemi Peng 
History: 08/03/2015 by Kemi Peng*/
/************************************************************************/
#ifndef WINDOW_WIN32
#define WINDOW_WIN32

#include "GearsCommonHead.h"
#include "WindowCallBackManager.h"

LRESULT WINAPI WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

class Window_Win32
{

public:
	Window_Win32( void );
	~Window_Win32( void );

	static void CreateEngineWindow( void );
	static Window_Win32 *GetEngineWindow( void );
	static void ReleaseEngineWindow( void );

	inline void setWindowHandleWin32( HWND hWnd ){ m_hWnd = hWnd; };
	inline HWND getWindowHandleWin32( void ) const { return m_hWnd; };
	inline HINSTANCE getWindowInstanceWin32( void ) const { return m_hInstance; };
	inline void getWindowSize( int &w, int &h ) const { w = m_windowWidth; h = m_windowHeight; };
	inline void setWindosPos( int x, int y ){ m_windowPosX = x; m_windowPosY = y; };
	inline void setWindosWandH( int w, int h ){ m_windowWidth = w; m_windowHeight = h; };
	inline WindowCallBackManager * getCallBackHandle( void ) const{ return m_pCallBackHandle; };

	void closeWindow( void );
	bool processMessage(void);
	bool fullScreen(int width, int height, const std::string &titlewstring );
	bool fullScreen( const std::string &titlewstring );
	bool createWindow(int x, int y, int width, int height, const std::string &titlewstring );
	bool createWindow( const std::string &titlewstring );

protected:
	unsigned long m_threadID;
	HWND m_hWnd;
	HINSTANCE m_hInstance;
	HANDLE m_threadHandle;
	WindowCallBackManager *m_pCallBackHandle;

	int m_windowPosX;
	int m_windowPosY;
	int m_windowWidth, m_windowWidthBack;
	int m_windowHeight, m_windowHeightBack;

	bool m_onSize;
	
};

extern Window_Win32* gWindow_Win32;		// store some information about this window, barely use.

#endif
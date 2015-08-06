/************************************************************************/
/* Author:  Kemi Peng 
History: 08/03/2015 by Kemi Peng*/
/************************************************************************/

#include "Window_Win32.h"
#include "UtilityManager.h"
Window_Win32* gWindow_Win32 = NULL;
LRESULT WINAPI WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY:
		{
			DEBUG_PRINT("WM_DESTROY...");
			PostQuitMessage(0);
			break;
		}

	case WM_SIZE:
		{
			DEBUG_PRINT("WM_SIZE...");
			int w = LOWORD(lParam);
			int h = HIWORD(lParam);
			int extraWidth  = GetSystemMetrics( SM_CXSIZEFRAME ) * 2;
			int extraHeight = GetSystemMetrics( SM_CYCAPTION ) + GetSystemMetrics( SM_CYSIZEFRAME ) * 2;
			Window_Win32::GetEngineWindow()->setWindosWandH( w+extraWidth, h+extraHeight );

			SetWindowWH(w, h);

			Window_Win32::GetEngineWindow()->getCallBackHandle()->onResetDevice( );

			break;
		}
	case WM_EXITSIZEMOVE:
		{
			DEBUG_PRINT("WM_EXITSIZEMOVE...");
			int w = LOWORD(lParam);
			int h = HIWORD(lParam);
			Window_Win32::GetEngineWindow()->setWindosPos( w, h );

			int width, height;
			Window_Win32::GetEngineWindow()->getWindowSize(width,height);
			Window_Win32::GetEngineWindow()->setWindosWandH( width, height );

			Window_Win32::GetEngineWindow()->getCallBackHandle()->onResetDevice( );
			break;
		}

	case WM_PAINT: 
		{
			PAINTSTRUCT ps;
			BeginPaint(hwnd, &ps);
			EndPaint(hwnd, &ps);

			// gWindow_Win32->getCallBackHandle().onPaint();

			break;
		}

	default: 
		{
			return DefWindowProc(hwnd, message, wParam, lParam);
			break;
		}
	}

	return 0;
}

Window_Win32::Window_Win32( void ):
								m_hWnd( NULL ),
								m_hInstance( NULL ),
								m_threadHandle( NULL ),
								m_threadID( 0 ),
								m_windowPosX( 100 ),
								m_windowPosY( 100 ),
								m_windowWidth( 1280 ),
								m_windowHeight( 720 ),
								m_onSize( false ),
								m_pCallBackHandle( new WindowCallBackManager )
{
}

Window_Win32::~Window_Win32( void )
{
	SAFE_DELETE(m_pCallBackHandle);
}


void Window_Win32::CreateEngineWindow( void )
{
	if ( gWindow_Win32 == NULL )
	{
		gWindow_Win32 = new Window_Win32();
	}
}

void Window_Win32::closeWindow( void )
{
	PostQuitMessage(0);
}

bool Window_Win32::processMessage( void )
{
	MSG	msg;
	if ( PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) )
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		if ( msg.message==WM_QUIT )
		{
			return false;
		}
	}

	return true;
}

bool Window_Win32::fullScreen( int width, int height, const std::string &titlestring )
{
	const char *title = titlestring.c_str();

	DEVMODE dmScreenSettings;								// Device Mode
	memset(&dmScreenSettings,0,sizeof(dmScreenSettings));	// Makes Sure Memory's Cleared
	dmScreenSettings.dmSize=sizeof(dmScreenSettings);		// Size Of The Devmode Structure
	dmScreenSettings.dmPelsWidth	= width;				// Selected Screen Width
	dmScreenSettings.dmPelsHeight	= height;				// Selected Screen Height
	dmScreenSettings.dmBitsPerPel	= 32;					// Selected Bits Per Pixel
	dmScreenSettings.dmFields=DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;

	// Try To Set Selected Mode And Get Results.  NOTE: CDS_FULLSCREEN Gets Rid Of Start Bar.
	if (ChangeDisplaySettings(&dmScreenSettings,CDS_FULLSCREEN)!=DISP_CHANGE_SUCCESSFUL)
	{
		// If The Mode Fails, Offer Two Options.  Quit Or Use Windowed Mode.
		return false;
	}


	WNDCLASS	window_class;

	memset(&window_class, 0, sizeof(WNDCLASS));

	// window type
	window_class.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW ;
	window_class.lpfnWndProc =	WndProc;
	// get instance
	window_class.hInstance = GetModuleHandle(NULL);
	// default cursor
	window_class.hCursor = LoadCursor(NULL, IDC_ARROW);
	// black background
	window_class.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	// title
	window_class.lpszClassName = title;
	// register the window
	if (RegisterClass(&window_class) == 0)
		return 0;

	DWORD dwExStyle=WS_EX_APPWINDOW;	// Window Extended Style
	DWORD dwStyle=WS_POPUP;				// Windows Style
	ShowCursor(FALSE);					// Hide Mouse Pointer

	RECT		WindowRect;				// Grabs Rectangle Upper Left / Lower Right Values

	WindowRect.left=(long)0;			// Set Left Value To 0
	WindowRect.right=(long)width;		// Set Right Value To Requested Width
	WindowRect.top=(long)0;				// Set Top Value To 0
	WindowRect.bottom=(long)height;		// Set Bottom Value To Requested Height

	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);		// Adjust Window To True Requested Size

	m_windowPosX = m_windowPosY = 0;
	m_windowWidth = width;
	m_windowHeight = height;

	HWND window_handle = CreateWindowEx(	
		dwExStyle,
		title,
		title,
		dwStyle |
		WS_CLIPSIBLINGS |
		WS_CLIPCHILDREN,
		0, 0,
		WindowRect.right-WindowRect.left,
		WindowRect.bottom-WindowRect.top,
		NULL,
		NULL,
		window_class.hInstance,
		NULL
		);

	if (window_handle == NULL)
		return false;


	m_hWnd = window_handle;
	m_hInstance = window_class.hInstance;
	// show the window
	ShowWindow(window_handle, SW_SHOWNORMAL);
	SetActiveWindow(window_handle);
	SetForegroundWindow(window_handle);	
	SetFocus(window_handle);

	return true;
}


bool Window_Win32::fullScreen( const std::string &titlestring )
{
	return fullScreen(m_windowWidth, m_windowHeight, titlestring);
}

bool Window_Win32::createWindow( int x, int y, int width, int height, const std::string &titlestring )
{
	DEBUG_PRINT("create window...");
	static bool registered = false;

	WNDCLASS	window_class;

	const char *title = titlestring.c_str();

	memset(&window_class, 0, sizeof(WNDCLASS));

	// window type
	window_class.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;

	window_class.lpfnWndProc = WndProc;
	// get instance
	window_class.hInstance = GetModuleHandle(NULL);
	// default cursor
	window_class.hCursor = LoadCursor(NULL, IDC_ARROW);
	// black background
	window_class.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	// title
	window_class.lpszClassName = title;
	// register it.
	if ( !registered )
	{
		if (RegisterClass(&window_class) == 0)
			return false;
		registered = true;
	}

	DWORD window_style;

	if ( m_onSize )
	{
		window_style = WS_OVERLAPPEDWINDOW;
	}
	else
	{
		// no window size change allowed during running
		window_style = WS_BORDER | WS_SYSMENU;
	}

	if ( x<0 || y<0 )
	{
		x = m_windowPosX;
		y = m_windowPosY;
	}

	if ( width<0 || height < 0 )
	{
		width = m_windowWidth;
		height = m_windowHeight;
	}

	int extraWidth  = GetSystemMetrics( SM_CXSIZEFRAME ) * 2;
	int extraHeight = GetSystemMetrics( SM_CYCAPTION ) + GetSystemMetrics( SM_CYSIZEFRAME ) * 2;
	RECT window_rect;
	SetRect(&window_rect, x, y, x+width+extraWidth, y+height+extraHeight);
	AdjustWindowRect(&window_rect, window_style, FALSE);

	m_windowPosX = x;
	m_windowPosY = y;
	m_windowWidth = width;
	m_windowHeight = height;

	// `open the window`
	HWND window_handle = CreateWindowEx(
		WS_EX_APPWINDOW,
		title,
		title,
		window_style,
		window_rect.left,	// x
		window_rect.top,	// y
		window_rect.right - window_rect.left,	// width
		window_rect.bottom - window_rect.top,	// height
		NULL,
		NULL,
		window_class.hInstance,
		NULL
		);

	if (window_handle == NULL)
		return false;

	m_hWnd = window_handle;
	m_hInstance = window_class.hInstance;
	// display the window
	ShowWindow(window_handle, SW_SHOWNORMAL);
	SetActiveWindow(window_handle);

	return true;
}

bool Window_Win32::createWindow( const std::string &titlewstring )
{
 	return createWindow( m_windowPosX, m_windowPosY, m_windowWidth, m_windowHeight, titlewstring );
}

Window_Win32 *Window_Win32::GetEngineWindow( void )
{
	return gWindow_Win32;
}

void Window_Win32::ReleaseEngineWindow( void )
{
	SAFE_DELETE(gWindow_Win32);
}
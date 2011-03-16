#include <glee.h>
#include <cstdio>

#include <oleauto.h>
#include <WindowsX.h>

#include "poWindow.h"
#include "EventCodes.h"
#include "poApplication.h"

#ifndef PFD_SUPPORT_COMPOSITION
#define PFD_SUPPORT_COMPOSITION 0x00008000
#endif

static const char *window_class_name = "windows_class_its_awesome";

static const long BASIC_WINEX_FLAGS = WS_EX_OVERLAPPEDWINDOW;
static const long BASIC_WIN_FLAGS = WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_OVERLAPPEDWINDOW;
static const long BORDERLESS_WINEX_FLAGS = WS_EX_TOPMOST;
static const long BORDERLESS_WIN_FLAGS  = WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_POPUP;

// the window proc callback, where all the magic happens at runtime
static LONG WINAPI WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

// helper functions
static void registerWindowsClass(HINSTANCE hinst, WNDPROC proc);
static unsigned int mouseEventModifiers(WPARAM wParam);
static unsigned int keyEventModifiers();
static int nativeKeyCode(WPARAM wParam);
static char mapVirtualKey(WPARAM wParam);
static void xyForLparam(LPARAM param, int *x, int *y);
static short mouseWheelForWparam(WPARAM wparam);
static void ChooseBestAntiAliasingPixelFormat(int &pf);
static void ChooseAntiAliasingPixelFormat(int &pf, int samples);


// the actual window implementation for windows
struct Win32WindowImpl
:	public poWindow::WindowImpl 
{
	Win32WindowImpl(poWindow *win,
					poWindowType ty, 
					const std::string &t,
					int x, int y, int w, int h)
	{
		// configure the base
		window = win;
		type = ty;
		title = t;
		width = w;
		height = h;
		xpos = x;
		ypos = y;
		is_fullscreen = false;
		is_dragging = false;

		static int ID = 0;
		id = ID++;
	}

	~Win32WindowImpl() {
		// get rid of the opengl context
		wglMakeCurrent(hdc,NULL);
		wglDeleteContext(hglrc);
		ReleaseDC(hwnd, hdc);
		wglMakeCurrent(NULL, NULL);
		// remove the Window* from the hwnd
		SetWindowLongPtr(hwnd, GWL_USERDATA, (LONG)0);
		// post the destroy message
		DestroyWindow(hwnd);
	}
    
	void initialize() {
		// get the global instance handle
		hinst = GetModuleHandle(NULL);

		// configure the window creation flags
		switch(type) {
		case WINDOW_TYPE_FULLSCREEN:
		case WINDOW_TYPE_NORMAL:
			exflags = BASIC_WINEX_FLAGS;
			flags = BASIC_WIN_FLAGS;
			break;
		case WINDOW_TYPE_BORDERLESS:
			exflags = BORDERLESS_WINEX_FLAGS;
			flags = BORDERLESS_WIN_FLAGS;
			break;
		}

		static bool window_class_registered = false;
		if(!window_class_registered) {
			registerWindowsClass(GetModuleHandle(NULL), WindowProc);
			window_class_registered = true;
		}

		// make the window and opengl context
		hwnd = CreateWindowEx(exflags, window_class_name, title.c_str(), flags,
							  xpos, ypos, width, height, NULL, NULL, hinst, NULL);

		hdc = GetDC(hwnd);
		// get the best pixel format possible
		setPixelFormat();

		// make the context
		hglrc = wglCreateContext(hdc);
		// shared opengl context
		static HGLRC opengl_context_to_share = NULL;
		if(opengl_context_to_share == NULL) {
			// and store it for sharing
			opengl_context_to_share = hglrc;
		}
		else {
			wglShareLists(opengl_context_to_share, hglrc);
		}

		wglMakeCurrent(hdc,hglrc);
		// try to lock to the vsync
		wglSwapIntervalEXT(1);

		// and stash the window in the hwnd
		SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG)window);

		// show the window now
		ShowWindow(hwnd, SW_SHOW);
		SetForegroundWindow(hwnd);
		SetFocus(hwnd);

		if(type == WINDOW_TYPE_FULLSCREEN)
			setFullscreen(true);
	}

	bool setPixelFormat() {
		int pf = 0;
		PIXELFORMATDESCRIPTOR pfd = {0};
		OSVERSIONINFO osvi = {0};

		pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
		pfd.nVersion = 1;
		pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
		pfd.iPixelType = PFD_TYPE_RGBA;
		pfd.cColorBits = 32;
		pfd.cDepthBits = 16;
		pfd.cStencilBits = 8;
		pfd.iLayerType = PFD_MAIN_PLANE;

		osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
		if (!GetVersionEx(&osvi))
			return false;

		// When running under Windows Vista or later support desktop composition.
		if (osvi.dwMajorVersion > 6 || (osvi.dwMajorVersion == 6 && osvi.dwMinorVersion >= 0))
			pfd.dwFlags |=  PFD_SUPPORT_COMPOSITION;

		//kind of slows things down, render-wise, but looks nice
		ChooseBestAntiAliasingPixelFormat(pf);
		if(!pf) //a little faster, a little worse
			ChooseAntiAliasingPixelFormat(pf, 4);
		if(!pf) // fast, primitives look mediocre
			pf = ChoosePixelFormat(hdc, &pfd);

		if (!SetPixelFormat(hdc, pf, &pfd))
			return false;

		return true;	
	}

	void moveTo(int x, int y, int w, int h) {
		MoveWindow(hwnd, x, y, w, h, false);
	}

	void setFullscreen(bool b) {
		if(is_fullscreen != b) {
			is_fullscreen = b;

			if(is_fullscreen) {
				// save the old rect
				GetWindowRect(hwnd, &rect);
				// hide the window first to reduce flicker
				ShowWindow(hwnd, SW_HIDE);
				
				// adjust the window decoration before doing anything else
				SetWindowLongPtr(hwnd, GWL_EXSTYLE, BORDERLESS_WINEX_FLAGS);
				SetWindowLongPtr(hwnd, GWL_STYLE, BORDERLESS_WIN_FLAGS);
				
				// fill the monitor we're currently on
				MONITORINFO mi = {sizeof(MONITORINFO)};
				GetMonitorInfo(MonitorFromWindow(hwnd, MONITOR_DEFAULTTONEAREST), &mi);
				xpos = mi.rcMonitor.left;
				ypos = mi.rcMonitor.top;
				width = mi.rcMonitor.right - mi.rcMonitor.left;
				height = mi.rcMonitor.bottom - mi.rcMonitor.top;

				// move the window into position
				SetWindowPos(hwnd, HWND_TOPMOST, xpos, ypos, width, height, SWP_SHOWWINDOW | SWP_FRAMECHANGED);
			}
			else {
				// redecorate the window
				SetWindowLongPtr(hwnd, GWL_EXSTYLE, exflags);
				SetWindowLongPtr(hwnd, GWL_STYLE, flags);

				// restore the old rect
				xpos = rect.left;
				ypos = rect.top;
				width = rect.right - rect.left;
				height = rect.bottom - rect.top;

				// move the window into position
				SetWindowPos(hwnd, HWND_TOP, xpos, ypos, width, height, SWP_SHOWWINDOW | SWP_FRAMECHANGED);
			}
		}
	}

	void startDraw() {
		// update the app
		wglMakeCurrent(hdc, hglrc);
	}

	void endDraw() {
		SwapBuffers(hdc);
		wglMakeCurrent(NULL, NULL);
	}

	HINSTANCE	hinst;
	HWND		hwnd;
	HDC			hdc;
	HGLRC		hglrc;
	RECT		rect;
	int			exflags;
	int			flags;
};

typedef Win32WindowImpl WINDOW_IMPL_TYPE;


LONG WINAPI WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	// retreive the window pointer associated with this hwnd
	poWindow *window = (poWindow*)GetWindowLongPtr(hwnd, GWL_USERDATA);
	if(!window) {
		// see if its time to close the application
		if(uMsg == WM_DESTROY && !poApplication::get()->numWindows())
			PostQuitMessage(0);

		// this is something like WM_CREATE that we don't care about 
		// so go head and bounce to the default
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

	switch(uMsg) {
	case WM_CLOSE:
		// take it out of the window list
		poApplication::get()->removeWindow(window);
		// will post a destroy message
		delete window;
		break;

	case WM_SIZE:
		window->setSize(LOWORD(lParam), HIWORD(lParam));
		break;

	// set the events flags for mouse events, then forward to the window 
	case WM_RBUTTONDOWN:
		::SetCapture(hwnd);
		{
			unsigned int mods = mouseEventModifiers(wParam);
			int x, y;
			xyForLparam(lParam, &x, &y);
			window->mouseDown(x, y, mods);
		}
		window->setDragging(true);
		break;
	case WM_RBUTTONUP:
		::ReleaseCapture();
		{
			unsigned int mods = mouseEventModifiers(wParam);
			int x, y;
			xyForLparam(lParam, &x, &y);
			window->mouseUp(x, y, mods);
		}
		window->setDragging(false);
		break;
	case WM_LBUTTONDOWN:
		::SetCapture(hwnd);
		{
			unsigned int mods = mouseEventModifiers(wParam);
			int x, y;
			xyForLparam(lParam, &x, &y);
			window->mouseDown(x, y, mods);
		}
		window->setDragging(true);
		break;
	case WM_LBUTTONUP:
		::ReleaseCapture();
		{
			unsigned int mods = mouseEventModifiers(wParam);
			int x, y;
			xyForLparam(lParam, &x, &y);
			window->mouseUp(x, y, mods);
		}
		window->setDragging(false);                
		break;
	case WM_MBUTTONDOWN:
		::SetCapture(hwnd);
		{
			unsigned int mods = mouseEventModifiers(wParam);
			int x, y;
			xyForLparam(lParam, &x, &y);
			window->mouseDown(x, y, mods);
		}
		window->setDragging(true);         
		break;
	case WM_MBUTTONUP:
		::ReleaseCapture();
		{
			unsigned int mods = mouseEventModifiers(wParam);
			int x, y;
			xyForLparam(lParam, &x, &y);
			window->mouseUp(x, y, mods);
		}
		window->setDragging(false);
		break;

	case WM_MOUSEMOVE:
		{
			unsigned int mods = mouseEventModifiers(wParam);
			int x, y;
			xyForLparam(lParam, &x, &y);

			if( window->isDragging() ) {
				// send drag events to the window
				window->mouseDrag(x, y, mods);
			}
			else {
				// send move events to the window instead
				window->mouseMove(x, y, mods);
			}
		}
		break;

	case WM_MOUSEWHEEL:
		{
			unsigned int mods = mouseEventModifiers(wParam);
			short wheel = mouseWheelForWparam(wParam);
			short num_steps = wheel / WHEEL_DELTA;
			int x, y;
			xyForLparam(lParam, &x, &y);
			window->mouseWheel(x, y, mods, num_steps);
		}
		break;

	case WM_SYSKEYDOWN: 
		{
			// alt-f4 ... pass to close the window
			bool alt_down = (GetKeyState(VK_LMENU) & 0x80) || (GetKeyState(VK_RMENU) & 0x80) || (GetKeyState(VK_MENU) & 0x80);
			if(VK_F4 == wParam && alt_down) {
				PostMessage(hwnd, WM_CLOSE, 0, 0);
				break;
			}
		}
	case WM_KEYDOWN:
		// trap F1 and tie make it toggle fullscreen
		if(VK_F1 == wParam) {
			window->toggleFullscreen();
		}
		else if(VK_ESCAPE == wParam) {
			poApplication::get()->quit();
		}
		else {
			unsigned int mods = keyEventModifiers();
			char key = mapVirtualKey(wParam);
			window->keyDown(key, mods);
		}
		break;

	case WM_SYSKEYUP:
	case WM_KEYUP:
		{
			unsigned int mods = keyEventModifiers();
			char key = mapVirtualKey(wParam);
			window->keyUp(key, mods);
		}
		break;

	case WM_KILLFOCUS:
		if(window->isDragging()) {
			// if we lose capture during a drag, post a mouse up event as a notifier
			window->mouseUp(0, 0, 0);
		}
		window->setDragging(false);
		break;

	//case WM_ACTIVATE:
	//	// make sure our fullscreen window plays nice with the task switcher
	//	if(window->is_fullscreen) {
	//		// if we're loosing focus, minimize to system tray
	//		bool is_deactivated = WA_INACTIVE == LOWORD(wParam);
	//		if(is_deactivated)
	//			ShowWindow(hwnd,SW_SHOWMINIMIZED);
	//		else
	//			// sw_showmaximized flashes the title bar
	//			// sw_show flashes the system tray, go figure
	//			ShowWindow(hwnd,SW_SHOWMAXIMIZED);
	//	}
	//	break;

	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

	return 0;
}

void registerWindowsClass(HINSTANCE hinst, WNDPROC proc) {
	WNDCLASSEX wcl = {0};
	wcl.cbSize = sizeof(wcl);
	wcl.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
	wcl.lpfnWndProc = proc;
	wcl.cbClsExtra = 0;
	wcl.cbWndExtra = 0;
	wcl.hInstance = hinst;
	wcl.hIcon = LoadIcon(0, IDI_APPLICATION);
	wcl.hCursor = LoadCursor(0, IDC_ARROW);
	wcl.hbrBackground = 0;
	wcl.lpszMenuName = 0;
	wcl.lpszClassName = window_class_name;
	wcl.hIconSm = 0;
	RegisterClassEx(&wcl);
}

unsigned int mouseEventModifiers(WPARAM wParam) {
	unsigned int result = 0;
	wParam = GET_KEYSTATE_WPARAM(wParam);
	if( wParam & MK_CONTROL ) result |= KEY_CTRL;
	if( wParam & MK_LBUTTON ) result |= MOUSE_LEFT;
	if( wParam & MK_MBUTTON ) result |= MOUSE_MIDDLE;
	if( wParam & MK_RBUTTON ) result |= MOUSE_RIGHT;
	if( wParam & MK_SHIFT ) result |= KEY_SHIFT;
	if( ::GetKeyState( VK_MENU ) < 0 ) result |= KEY_ALT;       
	if( (::GetKeyState( VK_LWIN ) < 0) || (::GetKeyState( VK_RWIN ) < 0) ) result |= KEY_META;
	return result;
}

unsigned int keyEventModifiers() {
	unsigned int result = 0;
	if( ::GetKeyState( VK_CONTROL ) & 0x8000 ) result |= KEY_CTRL;
	if( ::GetKeyState( VK_SHIFT ) & 0x8000 ) result |= KEY_SHIFT;
	if( ( ::GetKeyState( VK_LMENU ) & 0x8000 ) || ( ::GetKeyState( VK_RMENU ) & 0x8000 ) ) result |= KEY_ALT;   
	if( ( ::GetKeyState( VK_LWIN ) < 0 ) || ( ::GetKeyState( VK_RWIN ) < 0 ) ) result |= KEY_META;
	return result;
}

int nativeKeyCode(WPARAM wParam) {
	unsigned int result = (int)wParam;
	if( wParam == VK_MENU ) {
		result = ( ::GetKeyState( VK_RMENU ) ) ? VK_RMENU : VK_LMENU;
	}
	else if( wParam == VK_SHIFT ) {
		result = ( ::GetKeyState( VK_RSHIFT ) ) ? VK_RSHIFT : VK_LSHIFT;      
	}
	else if( wParam == VK_CONTROL ) {
		result = ( ::GetKeyState( VK_RCONTROL ) ) ? VK_RCONTROL : VK_LCONTROL;                
	}
	return result;
}

char mapVirtualKey(WPARAM wParam) {
	BYTE keyboardState[256];
	::GetKeyboardState( keyboardState );
	WORD result[4];

	int resultLength = ::ToAscii( wParam, ::MapVirtualKey( wParam, 0 ), keyboardState, result, 0 );
	if( resultLength == 1 )
		return (char)result[0];
	return 0;
}

void xyForLparam(LPARAM param, int *x, int *y) {
	*x = GET_X_LPARAM(param);
	*y = GET_Y_LPARAM(param);
}

short mouseWheelForWparam(WPARAM wparam) {
	return GET_WHEEL_DELTA_WPARAM(wparam);
}

// private namespace for some helper functions to get the best pixel format possible
namespace {

	WNDCLASSEX g_wcl;
	HWND g_hWnd;
	HDC g_hDC;
	HGLRC g_hRC;
	char g_szAAPixelFormat[32];

	LRESULT CALLBACK DummyGLWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		switch (msg)
		{
		case WM_CREATE:
			if (!(g_hDC = GetDC(hWnd)))
				return -1;
			break;

		case WM_DESTROY:
			if (g_hDC)
			{
				if (g_hRC)
				{
					wglMakeCurrent(g_hDC, 0);
					wglDeleteContext(g_hRC);
					g_hRC = 0;
				}

				ReleaseDC(hWnd, g_hDC);
				g_hDC = 0;
			}

			PostQuitMessage(0);
			return 0;

		default:
			break;
		}

		return DefWindowProc(hWnd, msg, wParam, lParam);
	}

	bool CreateDummyGLWindow()
	{
		g_wcl.cbSize = sizeof(g_wcl);
		g_wcl.style = CS_OWNDC;
		g_wcl.lpfnWndProc = DummyGLWndProc;
		g_wcl.hInstance = reinterpret_cast<HINSTANCE>(GetModuleHandle(0));
		g_wcl.lpszClassName = "DummyGLWindowClass";

		if (!RegisterClassEx(&g_wcl))
			return false;

		g_hWnd = CreateWindow(g_wcl.lpszClassName, "", WS_OVERLAPPEDWINDOW,
			0, 0, 0, 0, 0, 0, g_wcl.hInstance, 0);

		if (!g_hWnd)
			return false;

		PIXELFORMATDESCRIPTOR pfd = {0};

		pfd.nSize = sizeof(pfd);
		pfd.nVersion = 1;
		pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL;
		pfd.iPixelType = PFD_TYPE_RGBA;
		pfd.cColorBits = 24;
		pfd.cDepthBits = 16;
		pfd.iLayerType = PFD_MAIN_PLANE;

		int pf = ChoosePixelFormat(g_hDC, &pfd);

		if (!SetPixelFormat(g_hDC, pf, &pfd))
			return false;

		if (!(g_hRC = wglCreateContext(g_hDC)))
			return false;

		if (!wglMakeCurrent(g_hDC, g_hRC))
			return false;

		return true;
	}

	void ChooseBestCSAAPixelFormat(int &pf)
	{
		struct CSAAPixelFormat
		{
			int numColorSamples;
			int numCoverageSamples;
			const char *pszDescription;
		};

		CSAAPixelFormat csaaPixelFormats[] =
		{
			{ 4, 8,  "8x CSAA" },
			{ 4, 16, "16x CSAA" },
			{ 8, 8,  "8xQ (Quality) CSAA" },
			{ 8, 16, "16xQ (Quality) CSAA" }
		};

		int totalCSAAFormats = static_cast<int>(sizeof(csaaPixelFormats) /
			sizeof(CSAAPixelFormat));

		int attributes[] =
		{
			WGL_SAMPLE_BUFFERS_ARB,  1,
			WGL_DOUBLE_BUFFER_ARB,   1,
			0, 0
		};

		int returnedPixelFormat = 0;
		UINT numFormats = 0;
		BOOL bStatus = FALSE;

		for (int i = totalCSAAFormats - 1; i >= 0; --i)
		{
			attributes[3] = csaaPixelFormats[i].numColorSamples;
			attributes[5] = csaaPixelFormats[i].numCoverageSamples;

			bStatus = wglChoosePixelFormatARB(g_hDC, attributes, 0, 1,
				&returnedPixelFormat, &numFormats);

			if (bStatus == TRUE && numFormats)
			{
				pf = returnedPixelFormat;
				strcpy(g_szAAPixelFormat, csaaPixelFormats[i].pszDescription);
				break;
			}
		}

		if (bStatus == FALSE)
			g_szAAPixelFormat[0] = '\0';
	}

	void ChooseBestMSAAPixelFormat(int &pf)
	{
		int attributes[] =
		{
			WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
			WGL_ACCELERATION_ARB,   WGL_FULL_ACCELERATION_ARB,
			WGL_COLOR_BITS_ARB,     24,
			WGL_ALPHA_BITS_ARB,     8,
			WGL_DEPTH_BITS_ARB,     24,
			WGL_STENCIL_BITS_ARB,   8,
			WGL_DOUBLE_BUFFER_ARB,  GL_TRUE,
			WGL_SAMPLE_BUFFERS_ARB, GL_TRUE,
			WGL_SAMPLES_ARB,        0,
			0, 0
		};

		int returnedPixelFormat = 0;
		UINT numFormats = 0;
		BOOL bStatus = FALSE;

		for (int samples = 16; samples > 0; samples /= 2)
		{
			attributes[17] = samples;

			bStatus = wglChoosePixelFormatARB(g_hDC, attributes, 0, 1,
				&returnedPixelFormat, &numFormats);

			if (bStatus == TRUE && numFormats)
			{
				pf = returnedPixelFormat;
				sprintf(g_szAAPixelFormat, "%dx MSAA", samples);
				break;
			}
		}

		if (bStatus == FALSE)
			g_szAAPixelFormat[0] = '\0';
	}

	void ChooseCSAAPixelFormat(int &pf, int samples)
	{
		struct CSAAPixelFormat
		{
			int numColorSamples;
			int numCoverageSamples;
			const char *pszDescription;
		};

		CSAAPixelFormat csaaPixelFormats[] =
		{
			{ 4, 16, "16x CSAA" },
			{ 4, 8,  "8x CSAA" }
		};

		CSAAPixelFormat csaaQualityPixelFormats[] =
		{
			{ 8, 16, "16xQ (Quality) CSAA" },
			{ 8, 8,  "8xQ (Quality) CSAA" }
		};

		CSAAPixelFormat *pCSAAFormats = 0;

		int attributes[] =
		{
			WGL_SAMPLE_BUFFERS_ARB,  1,
			WGL_DOUBLE_BUFFER_ARB,   1,
			0, 0
		};

		int returnedPixelFormat = 0;
		UINT numFormats = 0;
		BOOL bStatus = FALSE;

		if (samples >= 8)
			pCSAAFormats = csaaQualityPixelFormats;
		else
			pCSAAFormats = csaaPixelFormats;

		for (int i = 0; i < 2; ++i)
		{
			attributes[3] = pCSAAFormats[i].numColorSamples;
			attributes[5] = pCSAAFormats[i].numCoverageSamples;

			bStatus = wglChoosePixelFormatARB(g_hDC, attributes, 0, 1,
				&returnedPixelFormat, &numFormats);

			if (bStatus == TRUE && numFormats)
			{
				pf = returnedPixelFormat;
				strcpy(g_szAAPixelFormat, pCSAAFormats[i].pszDescription);
				break;
			}
		}

		if (bStatus == FALSE)
			g_szAAPixelFormat[0] = '\0';
	}

	void ChooseMSAAPixelFormat(int &pf, int samples)
	{
		int attributes[] =
		{
			WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
			WGL_ACCELERATION_ARB,   WGL_FULL_ACCELERATION_ARB,
			WGL_COLOR_BITS_ARB,     24,
			WGL_ALPHA_BITS_ARB,     8,
			WGL_DEPTH_BITS_ARB,     24,
			WGL_STENCIL_BITS_ARB,   8,
			WGL_DOUBLE_BUFFER_ARB,  GL_TRUE,
			WGL_SAMPLE_BUFFERS_ARB, GL_TRUE,
			WGL_SAMPLES_ARB,        samples,
			0, 0
		};

		int returnedPixelFormat = 0;
		UINT numFormats = 0;
		BOOL bStatus = wglChoosePixelFormatARB(g_hDC, attributes, 0, 1,
			&returnedPixelFormat, &numFormats);

		if (bStatus == TRUE && numFormats)
		{
			pf = returnedPixelFormat;
			sprintf(g_szAAPixelFormat, "%dx MSAA", samples);
		}
		else
		{
			g_szAAPixelFormat[0] = '\0';
		}
	}

	void DestroyDummyGLWindow()
	{
		if (g_hWnd)
		{
			PostMessage(g_hWnd, WM_CLOSE, 0, 0);

			BOOL bRet;
			MSG msg;

			while ((bRet = GetMessage(&msg, 0, 0, 0)) != 0)
			{ 
				TranslateMessage(&msg); 
				DispatchMessage(&msg); 
			}
		}        

		UnregisterClass(g_wcl.lpszClassName, g_wcl.hInstance);
	}
}

void ChooseBestAntiAliasingPixelFormat(int &pf) {
	pf = 0;

	if (!CreateDummyGLWindow())
	{
		DestroyDummyGLWindow();
		return;
	}

	ChooseBestCSAAPixelFormat(pf);

	if(!pf)
		ChooseBestMSAAPixelFormat(pf);

	DestroyDummyGLWindow();
}

void ChooseAntiAliasingPixelFormat(int &pf, int samples) {
	pf = 0;

	if (!CreateDummyGLWindow())
	{
		DestroyDummyGLWindow();
		return;
	}

	ChooseCSAAPixelFormat(pf, samples);

	if(!pf)
		ChooseMSAAPixelFormat(pf, samples);

	DestroyDummyGLWindow();
}

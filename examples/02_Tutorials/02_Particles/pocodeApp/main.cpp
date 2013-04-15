#include "poWindow.h"

#include "pixel_format_helper.h"

#include "shellapi.h"
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include <io.h>
#include <fcntl.h>
 

#define LOSHORT(l) ((SHORT)(l))
#define HISHORT(l) ((SHORT)(((DWORD)(l) >> 16) & 0xFFFF))
 
static LPTSTR WND_CLASS_NAME = _T("OPENGL_WINDOW_CLASS");

RECT win_rect;
HINSTANCE hinst;
HWND hwnd;
HDC dc;
HGLRC rc;
HICON small_ico, large_ico;

int window_width = 0;
int window_height = 0;
bool isDragging = false;
bool is_fullscreen = false;

#ifdef _DEBUG
const static bool is_debug = true;
#else
const static bool is_debug = false;
#endif

void dbgPrintf(const char* fmt, ...) {
	static char buf[1024];

	va_list args;
	va_start(args, fmt);
	vsprintf(buf, fmt, args);
	va_end(args);

	OutputDebugStringA(buf);
}

LONG WINAPI WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

void registerWindowsClass(HINSTANCE hinst) {
	WNDCLASSEX wcl = {0};
	wcl.cbSize = sizeof(wcl);
	wcl.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
	wcl.lpfnWndProc = WindowProc;
	wcl.cbClsExtra = 0;
	wcl.cbWndExtra = 0;
	wcl.hInstance = hinst;
	wcl.hIcon = LoadIcon(0, IDI_APPLICATION);
	wcl.hCursor = LoadCursor(0, IDC_ARROW);
	wcl.hbrBackground = 0;
	wcl.lpszMenuName = 0;
	wcl.lpszClassName = WND_CLASS_NAME;
	wcl.hIconSm = 0;
	RegisterClassEx(&wcl);
}

bool SetWindowPixelFormat(HDC hdc) {
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
	//ChooseBestAntiAliasingPixelFormat(pf);
	//if(!pf)
	//	 //a little faster, a little worse
	ChooseAntiAliasingPixelFormat(pf, 4);
	if(!pf)
		// fast, primitives look mediocre
		pf = ChoosePixelFormat(hdc, &pfd);

	if (!SetPixelFormat(hdc, pf, &pfd))
		return false;

	return true;
}

void createWindow(LPTSTR title, float w, float h) {
	int flags = WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_OVERLAPPEDWINDOW;

	hwnd = CreateWindowEx(	WS_EX_APPWINDOW | WS_EX_WINDOWEDGE,
							WND_CLASS_NAME, title, flags,
							0, 0, w, h, 
							NULL, NULL, hinst, NULL);

	GetClientRect(hwnd, &win_rect);

	dc = GetDC(hwnd);
	SetWindowPixelFormat(dc);

	rc = wglCreateContext(dc);
	wglMakeCurrent(dc,rc);
	//wglSwapIntervalEXT(1);
}

void cleanupWindow() {
	wglMakeCurrent(dc,NULL);
	wglDeleteContext(rc);

	ReleaseDC(hwnd,dc);
	DestroyWindow(hwnd);
}

void openConsoleWindow() {
	AllocConsole();

    HANDLE handle_out = GetStdHandle(STD_OUTPUT_HANDLE);
    int hCrt = _open_osfhandle((long) handle_out, _O_TEXT);
    FILE* hf_out = _fdopen(hCrt, "w");
    setvbuf(hf_out, NULL, _IONBF, 1);
    *stdout = *hf_out;

    HANDLE handle_in = GetStdHandle(STD_INPUT_HANDLE);
    hCrt = _open_osfhandle((long) handle_in, _O_TEXT);
    FILE* hf_in = _fdopen(hCrt, "r");
    setvbuf(hf_in, NULL, _IONBF, 128);
    *stdin = *hf_in;
}

// returns milliseconds since bootup
double getTiming() {
	LARGE_INTEGER ticks_per_sec;
	LARGE_INTEGER tick;

	QueryPerformanceFrequency(&ticks_per_sec);
	QueryPerformanceCounter(&tick);
	return (tick.QuadPart / (double)ticks_per_sec.QuadPart) * 1000.0;
}

unsigned int prepMouseEventModifiers( WPARAM wParam )
{
	unsigned int result = 0;
	//if( wParam & MK_CONTROL ) result |= pop::MouseEvent::CTRL_DOWN;
	//if( wParam & MK_LBUTTON ) result |= pop::MouseEvent::LEFT_DOWN;
	//if( wParam & MK_MBUTTON ) result |= pop::MouseEvent::MIDDLE_DOWN;
	//if( wParam & MK_RBUTTON ) result |= pop::MouseEvent::RIGHT_DOWN;
	//if( wParam & MK_SHIFT ) result |= pop::MouseEvent::SHIFT_DOWN;
	//if( ::GetKeyState( VK_MENU ) < 0 ) result |= pop::MouseEvent::ALT_DOWN;	
	//if( (::GetKeyState( VK_LWIN ) < 0) || (::GetKeyState( VK_RWIN ) < 0) ) result |= pop::MouseEvent::META_DOWN;
	return result;
}

int prepNativeKeyCode( WPARAM wParam )
{
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

char mapVirtualKey( WPARAM wParam )
{
	BYTE keyboardState[256];
	::GetKeyboardState( keyboardState );
	WORD result[4];
	
	int resultLength = ::ToAscii( wParam, ::MapVirtualKey( wParam, 0 ), keyboardState, result, 0 );
	if( resultLength == 1 )
		return (char)result[0];
	else
		return 0;
}

unsigned int getKeyModifiers()
{
	unsigned int result = 0;
	if( ::GetKeyState( VK_CONTROL ) & 0x8000 ) result |= PO_KEY_CTRL;
	if( ::GetKeyState( VK_SHIFT ) & 0x8000 ) result |= PO_KEY_SHIFT;
	if( ( ::GetKeyState( VK_LMENU ) & 0x8000 ) || ( ::GetKeyState( VK_RMENU ) & 0x8000 ) ) result |= PO_KEY_ALT;	
	if( ( ::GetKeyState( VK_LWIN ) < 0 ) || ( ::GetKeyState( VK_RWIN ) < 0 ) ) result |= PO_KEY_META;
	return result;
}

void doFullscreen(HWND hwnd, bool b) {
	is_fullscreen = b;

	if(is_fullscreen) {
		GetWindowRect(hwnd, &win_rect);
		SetWindowLong(hwnd, GWL_STYLE, WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_POPUP);

		int screen_w = GetSystemMetrics(SM_CXSCREEN);
		int screen_h = GetSystemMetrics(SM_CYSCREEN);
		MoveWindow(hwnd, 0, 0, screen_w, screen_h, true);
		InvalidateRect(0,0,1);
		ShowWindow(hwnd, SW_SHOWMAXIMIZED);
		SetActiveWindow(hwnd);
		SetForegroundWindow(hwnd);
	}
	else {
		SetWindowLong(hwnd, GWL_STYLE, WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_OVERLAPPEDWINDOW);

		window_width = win_rect.right-win_rect.left;
		window_height = win_rect.bottom-win_rect.top;
		SetWindowPos(hwnd, NULL, win_rect.left, win_rect.top, window_width, window_height, SWP_FRAMECHANGED);
		InvalidateRect(0,0,1);
		ShowWindow(hwnd, SW_SHOWDEFAULT);
		SetForegroundWindow(hwnd);
		ShowCursor(true);
	}
}


LONG WINAPI WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	poWindow *win = (poWindow*)GetWindowLongPtr(hwnd, GWL_USERDATA);

	switch(uMsg) {
		case WM_DESTROY:
			PostQuitMessage(0);
			break;

		case WM_SIZE:
			window_width = LOWORD(lParam);
			window_height = HIWORD(lParam);
			if(win) win->resized(0,0,window_width,window_height);
			break;

		case WM_RBUTTONDOWN:
			::SetCapture(hwnd);
			isDragging = true;
			if(win) win->mouseDown(LOSHORT(lParam), HISHORT(lParam), 0);
			break;
		case WM_RBUTTONUP:
			::ReleaseCapture();
			isDragging = false;	
			if(win) win->mouseUp(LOSHORT(lParam), HISHORT(lParam), 0);
			break;

		case WM_LBUTTONDOWN:
			::SetCapture(hwnd);
			isDragging = true;
			if(win) win->mouseDown(LOSHORT(lParam), HISHORT(lParam), 0);
			break;
		case WM_LBUTTONUP:
			::ReleaseCapture();
			isDragging = false;
			if(win) win->mouseUp(LOSHORT(lParam), HISHORT(lParam), 0);
			break;

		case WM_MBUTTONDOWN:
			::SetCapture(hwnd);
			isDragging = true;
			if(win) win->mouseDown(LOSHORT(lParam), HISHORT(lParam), 0);
			break;
		case WM_MBUTTONUP:
			::ReleaseCapture();
			isDragging = false;	
			if(win) win->mouseUp(LOSHORT(lParam), HISHORT(lParam), 0);
			break;

		case WM_MOUSEMOVE:
			if( isDragging )
				if(win) win->mouseDrag(LOSHORT(lParam), HISHORT(lParam), 0);
			else
				if(win) win->mouseMove(LOSHORT(lParam), HISHORT(lParam), 0);
			break;

		case WM_MOUSEWHEEL:
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
			if(VK_F1 == wParam)
				doFullscreen(hwnd, !is_fullscreen);
			else
				if(win) win->keyDown(mapVirtualKey(wParam), wParam, getKeyModifiers());
			break;

		case WM_SYSKEYUP:
		case WM_KEYUP:
				if(win) win->keyUp(mapVirtualKey(wParam), wParam, getKeyModifiers());
			break;

		case WM_KILLFOCUS:
			// if we lose capture during a drag, post a mouse up event as a notifier
			if( isDragging ) {
				if(win) win->mouseDrag(LOSHORT(lParam), HISHORT(lParam), 0);
			}
			isDragging = false;
			break;

		case WM_ACTIVATE:
			// make sure our fullscreen window plays nice with the task switcher
			if(is_fullscreen) {
				// if we're loosing focus, minimize to system tray
				bool is_deactivated = WA_INACTIVE == LOWORD(wParam);
				if(is_deactivated)
					ShowWindow(hwnd,SW_SHOWMINIMIZED);
				else
					// sw_showmaximized flashes the title bar
					// sw_show flashes the system tray, go figure
					ShowWindow(hwnd,SW_SHOWMAXIMIZED);
			}
			break;

		default:
			return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

	return 0;
}

extern void setupApplication();
extern void cleanupApplication();
  
int APIENTRY WinMain(HINSTANCE hCurrentInst, HINSTANCE hPreviousInst, LPSTR lpszCmdLine, int nCmdShow) {

	openConsoleWindow();

	HINSTANCE hinst = hCurrentInst;  
	registerWindowsClass(hinst);
	setupApplication();

	double avg_frame_time = 0.0;
	double next_frame_time=-1.0, last_frame_time=0.0;
	int frame_num = 0;

	::ShowWindow( hwnd, SW_SHOW );
	::SetForegroundWindow( hwnd );
	::SetFocus( hwnd );

	// do the draw/update/event loop
	MSG msg;
	bool quit = false;
	while(!quit) {
		if( ::PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) ) {
			if(msg.message == WM_QUIT) {
				cleanupApplication();

				poWindow *win = (poWindow*)GetWindowLongPtr(hwnd,GWL_USERDATA);
				delete win;

				cleanupWindow();
				break;
			}

			::TranslateMessage( &msg );
			::DispatchMessage( &msg ); 
		}

		//double now = getTiming();
		//if(now >= next_frame_time) {
		//	next_frame_time = now + 1.0 / 60.0;
		//	double dt = now - last_frame_time;

		//	poWindow *win = (poWindow*)GetWindowLongPtr(hwnd,GWL_USERDATA);
		//	if(win) {
		//		win->makeCurrent();
		//		win->update();
		//		win->draw();
		//	}

		//	SwapBuffers(dc);

		//	frame_num++;
		//	last_frame_time = now;
		//}
		//else {
		//	Sleep(1);
		//}

		poWindow *win = (poWindow*)GetWindowLongPtr(hwnd,GWL_USERDATA);
		if(win) {
			win->makeCurrent();
			win->update();
			win->draw();
		}
		SwapBuffers(dc);
		Sleep(1);
	}

	cleanupWindow();
	
	#if defined WIN32 && defined _DEBUG
		//_CrtDumpMemoryLeaks();
	#endif	

	return msg.wParam;
}


poWindow *window = NULL;

int applicationNumberWindows() {
	return 1;
}

poWindow* applicationCreateWindow(uint uid, poWindowType type, const char* name, int x, int y, int w, int h) {
	if(window != NULL) {
		printf("only one window please!!\n");
		return window;
	}

	//createWindow((LPWSTR)name, w, h);
	createWindow((LPTSTR)L"pocodeApp", w, h);
	window = new poWindow(name,uid,poRect(x,y,w,h));

	RECT rcClient, rcWind;
	POINT ptDiff;
	GetClientRect(hwnd, &rcClient);
	GetWindowRect(hwnd, &rcWind);
	ptDiff.x = (rcWind.right - rcWind.left) - rcClient.right;
	ptDiff.y = (rcWind.bottom - rcWind.top) - rcClient.bottom;
	MoveWindow(hwnd,rcWind.left, rcWind.top, w + ptDiff.x, h + ptDiff.y, TRUE);

	window->setWindowHandle(hwnd);

	SetWindowLongPtr(hwnd,GWL_USERDATA,(LONG)window);

	return window;
}

poWindow* applicationGetWindow(int index) {
	return window;
}

std::string	applicationGetResourceDirectory() {
	return fs::current_path().string();
}

std::string applicationGetSupportDirectory() {
	return fs::current_path().string();
}

poWindow* applicationCurrentWindow() {
	return window;
}

void applicationMakeWindowCurrent(poWindow* win) {
	window = win;
}

void applicationMakeWindowFullscreen(poWindow* win, bool b) {
}

void applicationMoveWindow(poWindow* win,poPoint p) {
}

void applicationReshapeWindow(poWindow* win, poRect r) {
}

void applicationQuit() {
}

void poHideCursor() {}
void poShowCursor() {}
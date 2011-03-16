#include <windows.h>

#include <list>

#include "poApplication.h"
#include "PlatformHelpers.h"

#ifdef OPEN_CONSOLE
#include <stdio.h>
#include <io.h>
#include <fcntl.h>

void openConsole() {
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
#endif

int APIENTRY WinMain(HINSTANCE hCurrentInst, HINSTANCE hPreviousInst, LPSTR lpszCmdLine, int nCmdShow) {
#ifdef OPEN_CONSOLE
	openConsole();
#endif

	// make the application object, which will make its own windows
	poApplication *app = poApplication::get();

	// initialize app after gl context has been set up
	double next_frame_time=-1.0; 
	double last_frame_time=0.0;
	int frame_num = 0;

	// do the pump
	MSG msg;
	bool quit = false;
	while(!quit) {
		if( ::PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) ) {
			if(msg.message == WM_QUIT)
				break;

			::TranslateMessage( &msg );
			::DispatchMessage( &msg ); 
		}

		double now = getTime();
		if(now >= next_frame_time) {
			next_frame_time = now + 1.0 / app->framerate;
			double dt = now - last_frame_time;

			// move the application forward
			app->doUpdate();

			frame_num++;
			last_frame_time = now;
		}
		else {
			Sleep(1);
		}
	}

#ifdef OPEN_CONSOLE
	FreeConsole();
#endif

	return (int)msg.wParam;
}
#include <oleauto.h>
#include <stdio.h>
#include <tchar.h>
#include "pixel_format_helper.h"
  
namespace {
	 
	WNDCLASSEX g_wcl;
	HWND g_hWnd = 0;
	HDC g_hDC = 0;
	HGLRC g_hRC = 0;
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
		g_wcl.lpszClassName = _T("DummyGLWindowClass");

		if (!RegisterClassEx(&g_wcl))
			return false;

		g_hWnd = CreateWindow(g_wcl.lpszClassName, _T(""), WS_OVERLAPPEDWINDOW,
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

		// i guess we have to do this here when glew is statically linked
		// if its a dll this gets called when the dll is loaded
		if(glewInit() != GLEW_OK)
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

			bStatus = wglChoosePixelFormatARB(g_hDC, attributes, 0, 1, &returnedPixelFormat, &numFormats);

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
			attributes[1] = pCSAAFormats[i].numColorSamples;
			attributes[3] = pCSAAFormats[i].numCoverageSamples;

			bStatus = wglChoosePixelFormatARB(g_hDC, attributes, 0, 1, &returnedPixelFormat, &numFormats);

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

// MyRendererTest.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <windows.h>
#include <iostream>

HWND g_hwnd = NULL;
BOOL g_exit = FALSE;

#ifdef _UNICODE
#define out	std::wcout
#else
#define out std::cout
#endif // _UNICODE

//////////////////////////////////////////////////////////////////////////

void show_lasterr()
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		GetLastError(),
		0, // Default language
		(LPTSTR)&lpMsgBuf,
		0,
		NULL
	);
	// Process any inserts in lpMsgBuf.
	// ...
	// Display the string.
	MessageBox(NULL, (LPCTSTR)lpMsgBuf, _T("Error"), MB_OK | MB_ICONINFORMATION);
	// Free the buffer.
	LocalFree(lpMsgBuf);
}

LRESULT CALLBACK WindowProc(
	HWND hwnd,
	UINT uMsg,
	WPARAM wParam,
	LPARAM lParam
)
{
	switch (uMsg)
	{
	case WM_CLOSE:	// 16
		g_exit = TRUE;
		std::cout << uMsg << std::endl;
		break;
	case WM_KEYUP:	// 257
		std::cout << "wParam: " << wParam << std::endl;
		switch (wParam)
		{
		case VK_ESCAPE:	// 27
			if (g_hwnd) DestroyWindow(g_hwnd);
			g_exit = TRUE;
			return 0L;
		default:
			break;
		}
	default:
		break;
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);	
}

int window_init(int width, int height, const TCHAR* title)
{
	WNDCLASS wc = {
		CS_BYTEALIGNCLIENT,		// style
		(WNDPROC)WindowProc,	// lpfnWndProc,a long pointer to the window procedure.
		0,						// cbClsExtra
		0,						// cbWndExtra
		0,						// hInstance,Handle to the instance that the window procedure of this class is within
		0,						// hIcon
		0,						// hCursor	
		0,						// hbrBackground
		0,						// lpszMenuName
		_T("renderer")			// lpszClassName
	};
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.hInstance = GetModuleHandle(NULL);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);

	// registers a window class for subsequent use in calls to the CreateWindow or CreateWindowEx function.
	if (!RegisterClass(&wc)) return -1;

	g_hwnd = CreateWindow(
		_T("renderer"), // LPCTSTR lpClassName,
		title,			// LPCTSTR lpWindowName,
		WS_OVERLAPPED|WS_SYSMENU,	// DWORD dwStyle,
		0,				// int x,
		0,				// int y,
		500,			// int nWidth,
		400,			// int nHeight,
		NULL,			// HWND hWndParent,
		NULL,			// HMENU hMenu,
		wc.hInstance,	// HANDLE hInstance,
		NULL			// PVOID lpParam
	);
	if (!g_hwnd) {
		show_lasterr();
		return -2;
	}
	ShowWindow(g_hwnd, SW_NORMAL);
}


int main()
{
	window_init(0, 0, _T("myrenderer"));
	
	//while (!g_exit)
	{
		MSG msg;
		BOOL bRet = 0;
		//while (1) 
		while ((bRet = GetMessage(&msg, g_hwnd, 0, 0)) != 0)
		{
			//if (!PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)) break;
			//if (!GetMessage(&msg, NULL, 0, 0)) { 
			//	break; 
			//}
			if (bRet == -1)
			{
				break;
			}
			else {
				TCHAR text[255];
				GetWindowText(msg.hwnd, text, 255);
				out << msg.hwnd << _T(":") << text << _T("> ") << msg.message << std::endl;
				DispatchMessage(&msg);
			}			
		}
	}
	system("pause");
    return 0;
}


// firefox.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "ezander.h"
#include <vector>
#include <iostream>
#include <fstream>

using namespace std;

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name
HANDLE threadHandle;
vector<RECT> rects;
SIZE screenSize;


// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
DWORD WINAPI		MyThread(LPVOID);

int APIENTRY _tWinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPTSTR    lpCmdLine,
	int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_EZANDER, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_EZANDER));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage are only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_EZANDER));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_EZANDER);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND hWnd;

	hInst = hInstance; // Store instance handle in our global variable

	hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, 150, 150, NULL, NULL, hInstance, NULL);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_CREATE:
		{
			ifstream fin("cfg.cfg");
			int n = 0;
			fin >> n;
			rects.resize(n);
			for (int i = 0; i < n; ++i)
			{
				int w, h;
				fin >> rects[i].left >> rects[i].top >> w >> h;
				rects[i].right = rects[i].left + w;
				rects[i].bottom = rects[i].top + h;
			}

			screenSize.cx = GetSystemMetrics(SM_CXSCREEN);
			screenSize.cy = GetSystemMetrics(SM_CYSCREEN);

			SECURITY_ATTRIBUTES secAttr = {0};			
			secAttr.nLength = sizeof(secAttr);			
			threadHandle = CreateThread(&secAttr, 0, MyThread, NULL, CREATE_SUSPENDED, NULL);
			if (threadHandle != INVALID_HANDLE_VALUE)
				ResumeThread(threadHandle);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here...
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:		
		CloseHandle(threadHandle);
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

int rand(int maxx)
{
	return (int)(rand() * (double)maxx / RAND_MAX);
}

DWORD WINAPI MyThread(LPVOID) 
{
	static const int abs_val = 65535;
	while (true)
		if (GetAsyncKeyState('R') & 0x8000) {
			POINT p;
			GetCursorPos(&p);
			for (int i = 0, sz = rects.size(); i < sz; ++i)		{			
				const RECT& rc = rects[i];
				POINT newP = {rc.left + rand(rc.right - rc.left), rc.top + rand(rc.bottom - rc.top)};
				INPUT input = {0};
				input.mi.dx = newP.x - p.x;
				input.mi.dy = newP.y - p.y;
				input.mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE;
				//input.mi.time = hypot(input.mi.dx, input.mi.dy) / 50;
				input.mi.dx = (double)newP.x / screenSize.cx * abs_val;
				input.mi.dy = (double)newP.y / screenSize.cy * abs_val;
				p = newP;
				input.type = INPUT_MOUSE;
				SendInput(1, &input, sizeof(input));

				Sleep(rand(20));
				input.mi.dx = input.mi.dy = 0;
				input.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
				input.mi.time = 0;
				//input.mi.time = rand(20);
				SendInput(1, &input, sizeof(input));

				Sleep(rand(20));			
				input.mi.dwFlags = MOUSEEVENTF_RIGHTUP;
				SendInput(1, &input, sizeof(input));

				Sleep(50 + rand(50));
			}
			Sleep(2000);
		} else
			Sleep(100);

		return 0;
}
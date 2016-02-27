#include "stdafx.h"
#include "HDXWindow.h"

LRESULT CALLBACK DXWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);

		FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));

		EndPaint(hwnd, &ps);
		return 0;
	}
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}

	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}


HDXWindow::HDXWindow(wchar_t * title, LONG width, LONG heigth, HObject *parent)
{
	parent->addChild(this);
	this->instance = GetModuleHandle(NULL);
	const wchar_t CLASS_NAME[] = L"WindowClass";


	WNDCLASSEX wc = {};
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = DXWindowProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = this->instance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = CLASS_NAME;
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	RegisterClassEx(&wc);
	this->wndIsSet++;
	RECT wr = { 0, 0, width, heigth };
	AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);

	this->hwnd = CreateWindowEx(
		0,
		CLASS_NAME,
		title,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, wr.right - wr.left, wr.bottom - wr.top,
		NULL,
		NULL,
		this->instance,
		NULL
		);
}

HDXWindow::~HDXWindow()
{
}

void HDXWindow::show()
{
	ShowWindow(this->hwnd, SW_SHOW);
	UpdateWindow(this->hwnd);

	BOOL quit = FALSE;
	MSG msg = {};

	while (!quit)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			if (msg.message == WM_QUIT)
			{
				quit = TRUE;
			}
		}
		else
		{

		}
	}
}


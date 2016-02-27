#include "stdafx.h"
#include "HWindow.h"

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
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
	case WM_CLOSE:
	{
		if (MessageBox(hwnd, L"Really quit?", L"Fenêtre", MB_OKCANCEL) == IDOK)
		{
			DestroyWindow(hwnd);
		}
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

HWindow::HWindow(wchar_t *title) : HObject()
{
	this->instance = GetModuleHandle(NULL);
	const wchar_t CLASS_NAME[] = L"Fenêtre de test";

	WNDCLASS wc = {};
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = this->instance;
	wc.lpszClassName = CLASS_NAME;

	RegisterClass(&wc);

	this->hwnd = CreateWindowEx(
		0,
		CLASS_NAME,
		title,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		NULL,
		NULL,
		this->instance,
		NULL
		);
}

HWindow::~HWindow()
{
}

void HWindow::show()
{
	ShowWindow(this->hwnd, SW_SHOW);


	MSG msg = {};

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

}

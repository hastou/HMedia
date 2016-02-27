#pragma once
#include "HObject.h"
#include <Windows.h>

class HWindow :
	public HObject
{
public:
	HWindow(wchar_t *title = L"Window", LONG width = 200, LONG heigth = 200);
	~HWindow();
	void show();

protected:
	HINSTANCE instance;
	HWND hwnd;
	static UINT wndIsSet;
};


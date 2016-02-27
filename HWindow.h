#pragma once
#include "HObject.h"
#include <Windows.h>

class HWindow :
	public HObject
{
public:
	HWindow(wchar_t *title);
	~HWindow();
	void show();
private:
	HINSTANCE instance;
	HWND hwnd;
};


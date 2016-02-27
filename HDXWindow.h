#pragma once
#include "HWindow.h"

class HDXWindow :
	public HWindow
{
public:
	HDXWindow(wchar_t *title = L"Window", LONG width = 200, LONG heigth = 200);
	~HDXWindow();
	void show();

};


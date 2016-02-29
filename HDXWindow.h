#pragma once
#include "HWindow.h"
#include <windowsx.h>
#include <d3d11.h>

#include <D3DX11.h>
#include <D3DX10.h>

#pragma comment (lib, "d3d11.lib") 
#pragma comment (lib, "d3dx11.lib") 
#pragma comment (lib, "d3dx10.lib")


class HDXWindow :
	public HWindow
{
public:
	HDXWindow(wchar_t *title = L"Window", LONG width = 200, LONG heigth = 200, HObject *parent = NULL);
	~HDXWindow();
	void show();

	virtual void DrawScene();
	virtual void UpdateScene();
	virtual void InitScene();
	virtual void OnResize();
	virtual LRESULT MsgProc(UINT msg, WPARAM Param, LPARAM lParam);

protected:
	IDXGISwapChain *swapchain;
	ID3D11Device *device;
	ID3D11DeviceContext *deviceContext;
	ID3D11RenderTargetView *backbuffer;

	void InitD3D(LONG width, LONG heigth);
	void CleanD3D();
};


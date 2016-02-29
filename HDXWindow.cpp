#include "stdafx.h"
#include "HDXWindow.h"

LRESULT CALLBACK DXWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	printf("Test");
	HDXWindow *window = NULL;

	window = (HDXWindow*)GetWindowLongPtr(hwnd, GWLP_USERDATA);

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
	if (window)
	{
		return (window->MsgProc(uMsg, wParam, lParam));
	}
	else
	{
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
}


HDXWindow::HDXWindow(wchar_t * title, LONG width, LONG heigth, HObject *parent)
{

	if (parent)
	{
		parent->addChild(this);
	}
	this->instance = GetModuleHandle(NULL);
	const wchar_t CLASS_NAME[] = L"DXWindowClass";


	WNDCLASSEX wc = {};
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = DXWindowProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = this->instance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	//wc.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
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
		this
		);
	SetWindowLongPtr(hwnd, GWLP_USERDATA,(LONG_PTR) this);

	this->InitD3D(width, heigth);
	this->InitScene();
}

HDXWindow::~HDXWindow()
{
	this->CleanD3D();
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
		this->UpdateScene();
		this->DrawScene();
	}
}

void HDXWindow::DrawScene()
{
	this->deviceContext->ClearRenderTargetView(this->backbuffer, D3DXCOLOR(0.0f, 0.0f, 0.1f, 1.0f));

	this->swapchain->Present(0, 0);
}

void HDXWindow::UpdateScene()
{

}

void HDXWindow::InitScene()
{
}

void HDXWindow::OnResize()
{
}

LRESULT HDXWindow::MsgProc(UINT msg, WPARAM Param, LPARAM lParam)
{
	switch (msg)
	{
	case WM_SIZE:
		this->OnResize();
	}
	return DefWindowProc(hwnd, msg, Param, lParam);
}

void HDXWindow::InitD3D(LONG width, LONG heigth)
{
	DXGI_SWAP_CHAIN_DESC scd;

	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

	scd.BufferCount = 1;
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scd.OutputWindow = this->hwnd;
	scd.SampleDesc.Count = 4;
	scd.Windowed = TRUE;
	scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	D3D11CreateDeviceAndSwapChain(NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		NULL,
		NULL,
		NULL,
		D3D11_SDK_VERSION,
		&scd,
		&this->swapchain,
		&this->device,
		NULL,
		&this->deviceContext);

	ID3D11Texture2D *pBackBuffer;
	this->swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

	this->device->CreateRenderTargetView(pBackBuffer, NULL, &this->backbuffer);
	pBackBuffer->Release();

	this->deviceContext->OMSetRenderTargets(1, &this->backbuffer, NULL);

	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = width;
	viewport.Height = heigth;

	this->deviceContext->RSSetViewports(1, &viewport);

}

void HDXWindow::CleanD3D()
{
	this->swapchain->SetFullscreenState(FALSE, NULL);
	this->backbuffer->Release();
	this->swapchain->Release();
	this->device->Release();
	this->deviceContext->Release();
}

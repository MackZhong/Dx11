#include "pch.h"
#include "Dx11System.h"

wchar_t szWindowClass[] = L"DirectX11WindowClass";

static Dx11System* DxSystemHandle = nullptr;

Dx11System::Dx11System(HINSTANCE hInstance, LPCWSTR lpCmdLine)
	: m_hInstance(hInstance), m_strCmdLine(lpCmdLine)
{
	DxSystemHandle = this;
}

Dx11System::~Dx11System()
{
}

LRESULT Dx11System::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	switch (umsg) {
	case WM_KEYDOWN:
		m_Input.KeyDown((unsigned int)wparam);
		return 0;
	case WM_KEYUP:
		m_Input.KeyUp((unsigned int)wparam);
		return 0;
	case WM_SIZE:
	{
		if (lparam) {
			int width = LOWORD(lparam);
			int height = HIWORD(lparam);
			m_Graphics.Resize(width, height);
		}
	}
	return 0;
	default:
		return DefWindowProcW(hwnd, umsg, wparam, lparam);
	}
}

bool Dx11System::Initialize(LPCWSTR szTitle)
{
	if (!InitializeDxWindow(szTitle)) {
		OutputDebugStringW(L"DirectX Window Initialize fail.\n");
		return false;
	}

	if (!m_Input.Initialize(m_hWnd)) {
		OutputDebugStringW(L"DirectX Input System Initialize fail.\n");
		return false;
	}

	if (!m_Graphics.Initialize(m_hWnd, m_szWindow.cx, m_szWindow.cy, m_bFullScreen)) {
		OutputDebugStringW(L"DirectX Graphics Initialize fail.\n");
		return false;
	}

	return true;
}

void Dx11System::Run()
{
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	m_bRuning = true;
	while (m_bRuning) {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}


		if (WM_QUIT == msg.message)
		{
			OutputDebugStringW(L"Message loop quit.\n");
			m_bRuning = false;
		}
		else {
			if (!UpdateFrame()) {
				m_bRuning = false;
			}
		}
	}
}

void Dx11System::Destroy()
{
	m_Graphics.Destroy();
	m_Input.Destroy();
	DestroyDxWindow();
}

bool Dx11System::UpdateFrame()
{
	if (m_Input.IsKeyDown(VK_ESCAPE)) {
		return false;
	}

	if (!m_Graphics.UpdateFrame()) {
		return false;
	}

	return true;
}

DWORD WindowStyleEx(bool fullscreen) {
	if (fullscreen)
		return WS_EX_APPWINDOW | WS_EX_NOREDIRECTIONBITMAP;
	else
		return WS_EX_APPWINDOW/* | WS_EX_NOREDIRECTIONBITMAP*/ | WS_EX_OVERLAPPEDWINDOW;
}
DWORD WindowStyle(bool fullscreen) {
	if (fullscreen)
		return WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_POPUPWINDOW | WS_VISIBLE;
	else
		return WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_POPUPWINDOW | WS_VISIBLE | WS_OVERLAPPEDWINDOW;
}

bool Dx11System::InitializeDxWindow(LPCWSTR szTitle)
{
	WNDCLASSEXW wc;
	ZeroMemory(&wc, sizeof(WNDCLASSEX));
	wc.cbSize = sizeof(WNDCLASSEXW);
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = DxWndProc;
	wc.hInstance = m_hInstance;
	wc.hIcon = (HICON)LoadImageW(NULL, L"Dx11L.ico", IMAGE_ICON, 0, 0, LR_LOADFROMFILE | LR_DEFAULTSIZE | LR_SHARED);
	wc.hIconSm = (HICON)LoadImageW(NULL, L"small.ico", IMAGE_ICON, 0, 0, LR_LOADFROMFILE | LR_DEFAULTSIZE | LR_SHARED);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
	wc.lpszClassName = szWindowClass;            // 主窗口类名

	ATOM atom = RegisterClassExW(&wc);
	if (!atom) {
		return false;
	}

	if (m_bFullScreen) {
		m_szWindow.cx = GetSystemMetrics(SM_CXSCREEN);
		m_szWindow.cy = GetSystemMetrics(SM_CYSCREEN);
		DEVMODEW dmScreen;
		ZeroMemory(&dmScreen, sizeof(DEVMODEW));
		dmScreen.dmSize = sizeof(DEVMODEW);
		dmScreen.dmPelsWidth = m_szWindow.cx;
		dmScreen.dmPelsHeight = m_szWindow.cy;
		dmScreen.dmBitsPerPel = 32;
		dmScreen.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		ChangeDisplaySettingsExW(NULL, &dmScreen, NULL, CDS_FULLSCREEN, NULL);
	}
	m_hWnd = CreateWindowExW(WindowStyleEx(m_bFullScreen), szWindowClass, szTitle,
		WindowStyle(m_bFullScreen), CW_USEDEFAULT, CW_USEDEFAULT, m_szWindow.cx, m_szWindow.cy,
		NULL, NULL, m_hInstance, NULL);
	if (!m_hWnd) {
		return false;
	}

	ShowWindow(m_hWnd, SW_SHOW);
	SetForegroundWindow(m_hWnd);
	SetFocus(m_hWnd);
	//ShowCursor(false);

	return true;
}

void Dx11System::DestroyDxWindow()
{
	//ShowCursor(true);

	if (m_bFullScreen) {
		ChangeDisplaySettingsW(NULL, 0);
	}

	DestroyWindow(m_hWnd);
	m_hWnd = nullptr;

	UnregisterClass(szWindowClass, m_hInstance);
	m_hInstance = nullptr;
}

LRESULT CALLBACK DxWndProc(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	switch (umsg) {
	case WM_DESTROY:
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;
	default:
		return DxSystemHandle->MessageHandler(hwnd, umsg, wparam, lparam);
	}
}
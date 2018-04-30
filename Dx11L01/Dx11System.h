#pragma once
#include "Dx11Input.h"
#include "Dx11Graphics.h"

class Dx11System
{
private:
	HINSTANCE m_hInstance;
	std::wstring m_strCmdLine;
	HWND	m_hWnd;
	Dx11Input m_Input;
	Dx11Graphics m_Graphics;
	SIZE m_szWindow{ 800, 600 };
	bool m_bFullScreen{ false };
	bool m_bRuning{ false };

public:
	Dx11System(_In_ HINSTANCE hInstance, _In_ LPCWSTR lpCmdLine);
	~Dx11System();

public:
	LRESULT CALLBACK MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam);
	bool Initialize(LPCWSTR szTitle);
	void Run();
	void Destroy();

private:
	bool UpdateFrame();
	bool InitializeDxWindow(LPCWSTR szTitle);
	void DestroyDxWindow();
};

LRESULT CALLBACK DxWndProc(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam);

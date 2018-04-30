#include "pch.h"
#include "Dx11Graphics.h"

Dx11Graphics::Dx11Graphics()
{
}

Dx11Graphics::~Dx11Graphics()
{
}

bool Dx11Graphics::Initialize(HWND hWnd, int Width, int Height, bool fullscreen)
{
	if (!m_DxEngine.Initialize(hWnd, Width, Height, m_ScreenDepth, m_ScreenNear, m_vSync, fullscreen)) {
		MessageBoxW(hWnd, L"Directx 12 Engine Initialize Fail.", L"Initialize Error", MB_OK);
		return false;
	}

	return true;
}

void Dx11Graphics::Destroy()
{
	m_DxEngine.Destroy();
}

bool Dx11Graphics::UpdateFrame()
{
	if (!m_DxEngine.UpdateFrame()) {
		OutputDebugStringW(L"Update Frame Fail.\n");
		return false;
	}

	if (!Render()) {
		OutputDebugStringW(L"Graphics render Fail.\n");
		return false;
	}

	return true;
}

bool Dx11Graphics::Render()
{
	m_DxEngine.BeginScene(0.0f, 0.0f, 0.5f, 1.0f);

	m_DxEngine.Render();

	m_DxEngine.EndScene();

	return true;
}
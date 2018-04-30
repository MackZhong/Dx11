#pragma once
#include "D3D11Engine.h"

class Dx11Graphics
{
private:
	float m_ScreenDepth{ 1000.0f };
	float m_ScreenNear{ 0.1f };
	bool m_vSync{ true };
	D3D11Engine m_DxEngine;

public:
	Dx11Graphics();
	~Dx11Graphics();

public:
	bool Initialize(HWND hWnd, int Width, int Height, bool fullscreen);
	void Destroy();
	bool UpdateFrame();

private:
	bool Render();
};

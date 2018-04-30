#pragma once
#include "D3D11Engine.h"
#include "DxCamera.h"
#include "DxModel.h"

class Dx11Graphics
{
private:
	UINT m_Width{ 0 };
	UINT m_Height{ 0 };
	float m_ScreenDepth{ 1000.0f };
	float m_ScreenNear{ 0.1f };
	bool m_vSync{ true };
	D3D11Engine m_DxEngine;
	DxCamera m_Camera;
	DxColorShader m_ColorShader;
	std::vector<DxModel*> m_Models;

public:
	Dx11Graphics();
	~Dx11Graphics();

	void Resize(int width, int height);
public:
	bool Initialize(HWND hWnd, int Width, int Height, bool fullscreen);
	void Destroy();
	bool UpdateFrame();

private:
	bool LoadAssets();
	bool Render();
};

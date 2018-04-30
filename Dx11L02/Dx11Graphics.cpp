#include "pch.h"
#include "Dx11Graphics.h"

Dx11Graphics::Dx11Graphics()
{
}

Dx11Graphics::~Dx11Graphics()
{
}

void Dx11Graphics::Resize(int width, int height)
{
	for (std::vector<DxModel*>::iterator m = m_Models.begin(); m < m_Models.end(); m++) {
		(*m)->Resize((float)width / (float)height);
	}
}

bool Dx11Graphics::LoadAssets()
{
	float aspectRatio = static_cast<float>(m_Width) / static_cast<float>(m_Height);
	DxModel* pModel = new DxModel;
	if (!pModel->Initialize(m_DxEngine.Device(), aspectRatio)) {

		return false;
	}

	m_Models.push_back(pModel);

	return true;
}

bool Dx11Graphics::Initialize(HWND hWnd, int Width, int Height, bool fullscreen)
{
	m_Width = Width;
	m_Height = Height;

	if (!m_DxEngine.Initialize(hWnd, Width, Height, m_ScreenDepth, m_ScreenNear, m_vSync, fullscreen)) {
		MessageBoxW(hWnd, L"Directx 12 Engine Initialize Fail.", L"Initialize Error", MB_OK);
		return false;
	}

	m_Camera.SetPostiion(0.0f, 0.0f, -5.0f);

	if (!m_ColorShader.Initialize(m_DxEngine.Device(), hWnd)) {
		OutputDebugStringW(L"Color Shader initialize fail.");
		return false;
	}

	if (!LoadAssets()) {
		OutputDebugStringW(L"Load assets fail.");
		return false;
	}

	return true;
}

void Dx11Graphics::Destroy()
{
	m_DxEngine.Destroy();
	for (std::vector<DxModel*>::iterator m = m_Models.begin(); m < m_Models.end(); m++) {
		delete *m;
		(*m) = nullptr;
	}
	m_Models.clear();
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

	m_Camera.Render();
	XMMATRIX matWorld;
	m_DxEngine.GetWorldMatrix(matWorld);
	XMMATRIX matView;
	m_Camera.GetViewMatrix(matView);
	XMMATRIX matProjection;
	m_DxEngine.GetProjectionMatrix(matProjection);

	for (std::vector<DxModel*>::iterator m = m_Models.begin(); m < m_Models.end(); m++) {
		(*m)->Render(m_DxEngine.Context());
		m_ColorShader.Render(m_DxEngine.Context(), (*m)->IndexCount(), matWorld, matView, matProjection);
	}

	m_DxEngine.Render();

	m_DxEngine.EndScene();

	return true;
}
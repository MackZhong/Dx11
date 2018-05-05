#pragma once
#include "Dx11Model.h"
#include "Dx11Camera.h"

using namespace DirectX;
using namespace Microsoft::WRL;

class Dx11Engine
{
public:

private:
	HWND m_hWnd{ NULL };
	int m_Width{ 800 };
	int m_Height{ 600 };
	bool m_vSync{ false };
	DXGI_SAMPLE_DESC m_SampleDesc{ 1, 0 };
	DXGI_FORMAT m_BufferFormat{ DXGI_FORMAT_R8G8B8A8_UNORM };
	ComPtr<ID3D11Device>			m_pd3dDevice{ nullptr };
	ComPtr<ID3D11DeviceContext>	m_pImmediateContext{ nullptr };
	ComPtr<IDXGISwapChain>		m_pSwapChain{ nullptr };
	ComPtr<ID3D11RenderTargetView>	m_pRenderTargetView{ nullptr };
	ComPtr<ID3D11DepthStencilView> m_pDepthStencilView{ nullptr };
	ComPtr<ID3D11Texture2D>			m_pDepthStencilBuffer{ nullptr };
	ComPtr<ID3D11DepthStencilState>	m_pDepthStencilState{ nullptr };
	ComPtr < ID3D11RasterizerState>	m_pRasterizerState{ nullptr };
	ComPtr<ID3D11Buffer>	m_AppConstantBuffer{ nullptr };
	ComPtr<ID3D11Buffer>	m_FrameConstantBuffer{ nullptr };
	XMMATRIX m_ProjectionMatrix;
	std::unique_ptr<Dx11Camera> m_Camera{ nullptr };
	std::unique_ptr<Dx11Model>	m_Model{ nullptr };

private:
	DXGI_RATIONAL QueryRefreshRate() {
		return { 60, 1 };
	}

	// How To: Create a Device and Immediate Context
	HRESULT CreateDeviceandContext() {
		HRESULT hr;

		DXGI_SWAP_CHAIN_DESC sd{ 0 };
		sd.BufferCount = 1;
		sd.BufferDesc.Width = this->m_Width;
		sd.BufferDesc.Height = this->m_Height;
		sd.BufferDesc.Format = m_BufferFormat;
		sd.BufferDesc.RefreshRate = QueryRefreshRate();
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.OutputWindow = this->m_hWnd;
		sd.SampleDesc = m_SampleDesc;
		sd.Windowed = TRUE;

		//D3D_FEATURE_LEVEL  FeatureLevelsRequested = D3D_FEATURE_LEVEL_11_0;
		//UINT               numFeatureLevelsRequested = 1;
		//hr = D3D11CreateDeviceAndSwapChain(NULL,
		//	D3D_DRIVER_TYPE_HARDWARE,
		//	NULL,
		//	0,
		//	&FeatureLevelsRequested,
		//	numFeatureLevelsRequested,
		//	D3D11_SDK_VERSION,
		//	&sd,
		//	&g_pSwapChain,
		//	&g_pd3dDevice,
		//	&FeatureLevelsSupported,
		//	&g_pImmediateContext);

		//if (FAILED(hr))
		//{
		//	return hr;
		//}

		const D3D_FEATURE_LEVEL lvl[] = {
			D3D_FEATURE_LEVEL_11_1,
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_1,
			D3D_FEATURE_LEVEL_10_0,
			D3D_FEATURE_LEVEL_9_3,
			D3D_FEATURE_LEVEL_9_2,
			D3D_FEATURE_LEVEL_9_1
		};

		UINT createDeviceFlags = 0;
#ifdef _DEBUG
		createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

		D3D_FEATURE_LEVEL  FeatureLevelsSupported;
		hr = D3D11CreateDeviceAndSwapChain(nullptr,
			D3D_DRIVER_TYPE_HARDWARE,
			nullptr, createDeviceFlags,
			lvl, _countof(lvl),
			D3D11_SDK_VERSION,
			&sd,
			this->m_pSwapChain.ReleaseAndGetAddressOf(),
			this->m_pd3dDevice.ReleaseAndGetAddressOf(),
			&FeatureLevelsSupported,
			this->m_pImmediateContext.ReleaseAndGetAddressOf());
		if (FAILED(hr))
		{
			hr = D3D11CreateDeviceAndSwapChain(nullptr,
				D3D_DRIVER_TYPE_HARDWARE,
				nullptr, createDeviceFlags,
				&lvl[1], _countof(lvl) - 1,
				D3D11_SDK_VERSION,
				&sd,
				this->m_pSwapChain.ReleaseAndGetAddressOf(),
				this->m_pd3dDevice.ReleaseAndGetAddressOf(),
				&FeatureLevelsSupported,
				this->m_pImmediateContext.ReleaseAndGetAddressOf());
		}
		if (FAILED(hr)) {
			return hr;
		}

		{
			ComPtr<ID3D11Texture2D> pBackBuffer;

			// Get a pointer to the back buffer
			hr = this->m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)pBackBuffer.ReleaseAndGetAddressOf());

			// Create a render-target view
			this->m_pd3dDevice->CreateRenderTargetView(pBackBuffer.Get(), NULL,
				this->m_pRenderTargetView.ReleaseAndGetAddressOf());

			// Bind the view
			ID3D11RenderTargetView* targets[1];
			targets[0] = this->m_pRenderTargetView.Get();
			this->m_pImmediateContext->OMSetRenderTargets(1, targets, NULL);
		}

		{
			// create the depth buffer for use with depth/stencil view
			D3D11_TEXTURE2D_DESC depthStencilBufferDesc{ 0 };
			depthStencilBufferDesc.ArraySize = 1;
			depthStencilBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
			//depthStencilBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
			depthStencilBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
			depthStencilBufferDesc.Width = m_Width;
			depthStencilBufferDesc.Height = m_Height;
			depthStencilBufferDesc.MipLevels = 1;
			depthStencilBufferDesc.SampleDesc = m_SampleDesc;
			depthStencilBufferDesc.Usage = D3D11_USAGE_DEFAULT;
			hr = m_pd3dDevice->CreateTexture2D(&depthStencilBufferDesc, nullptr, this->m_pDepthStencilBuffer.ReleaseAndGetAddressOf());
			if (FAILED(hr)) {
				return hr;
			}

			// create depth stencil view
			//D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc{ 0 };
			hr = m_pd3dDevice->CreateDepthStencilView(this->m_pDepthStencilBuffer.Get(), nullptr, this->m_pDepthStencilView.ReleaseAndGetAddressOf());
		}

		{
			// Depth stencil state
			D3D11_DEPTH_STENCIL_DESC depthStencilStateDesc{ 0 };
			depthStencilStateDesc.DepthEnable = TRUE;
			depthStencilStateDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
			depthStencilStateDesc.DepthFunc = D3D11_COMPARISON_LESS;
			depthStencilStateDesc.StencilEnable = FALSE;
			hr = m_pd3dDevice->CreateDepthStencilState(&depthStencilStateDesc, this->m_pDepthStencilState.ReleaseAndGetAddressOf());
			if (FAILED(hr)) {
				return hr;
			}

			// Rasterizer state
			D3D11_RASTERIZER_DESC rasterizerDesc;
			ZeroMemory(&rasterizerDesc, sizeof(rasterizerDesc));
			rasterizerDesc.AntialiasedLineEnable = FALSE;
			rasterizerDesc.CullMode = D3D11_CULL_BACK;
			rasterizerDesc.DepthBias = 0;
			rasterizerDesc.DepthBiasClamp = 0.0f;
			rasterizerDesc.DepthClipEnable = TRUE;
			rasterizerDesc.FillMode = D3D11_FILL_SOLID;
			rasterizerDesc.FrontCounterClockwise = FALSE;
			rasterizerDesc.MultisampleEnable = FALSE;
			rasterizerDesc.ScissorEnable = FALSE;
			rasterizerDesc.SlopeScaledDepthBias = 0.0f;
			hr = m_pd3dDevice->CreateRasterizerState(&rasterizerDesc, this->m_pRasterizerState.ReleaseAndGetAddressOf());
			if (FAILED(hr)) return hr;
		}

		{
			// Setup the viewport
			D3D11_VIEWPORT vp;
			vp.Width = (FLOAT)this->m_Width;
			vp.Height = (FLOAT)this->m_Height;
			vp.MinDepth = 0.0F;
			vp.MaxDepth = 1.0F;
			vp.TopLeftX = 0.0F;
			vp.TopLeftY = 0.0F;
			this->m_pImmediateContext->RSSetViewports(1, &vp);
		}
		// https://www.3dgep.com/introduction-to-directx-11
		return hr;
	}

	HRESULT CreateResources() {
		HRESULT hr = S_OK;

		m_Camera = std::make_unique<Dx11Camera>();

		m_Model = std::make_unique<Dx11Model>();
		if (m_Model) {
			return m_Model->Initialize(m_pd3dDevice.Get());
		}

		return E_FAIL;
	}

public:
	Dx11Engine() {
	};

	~Dx11Engine() {};


	HRESULT Initialize(HWND hWnd, int width, int height) {
		this->m_hWnd = hWnd;
		this->m_Width = width;
		this->m_Height = height;

		HRESULT hr = this->CreateDeviceandContext();
		if (FAILED(hr)) {
			return hr;
		}
		return this->CreateResources();
	}

	void Destroy() {

	}

	HRESULT UpdateFrame(float deltaTime) {
		HRESULT hr = S_OK;
		if (m_Model) {
			m_Model->Update(deltaTime);
		}
		return hr;
	}

	void DrawFrame() const {
		FLOAT bkClr[4] = { 0.0F, 0.0F, 1.0F, 1.0F };
		this->m_pImmediateContext->ClearRenderTargetView(this->m_pRenderTargetView.Get(), bkClr);

		if (m_Model) {
			m_Model->Render(this->m_pImmediateContext.Get(), m_Camera->View(), m_ProjectionMatrix);
		}

		this->m_pSwapChain->Present(0, 0);
	}
};

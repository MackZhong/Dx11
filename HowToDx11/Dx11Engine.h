#pragma once
#include <d3d11_4.h>
#include <dxgi1_6.h>
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxguid.lib")
#include <DirectXMath.h>
using namespace DirectX;
#include <d3dcompiler.h>
#pragma comment(lib,"d3dcompiler.lib")

#include <wrl.h>
namespace MWRL = Microsoft::WRL;

class Dx11Engine
{
public:

private:
	// Define the data-type that
	// describes a vertex.
	struct SimpleVertexCombined
	{
		XMFLOAT3 Pos;
		XMFLOAT3 Color;
	};
	D3D11_INPUT_ELEMENT_DESC layout[2]{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	//{ L"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
	//{ L"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};


	HWND g_hWnd{ NULL };
	int m_Width{ 800 };
	int m_Height{ 600 };
	MWRL::ComPtr<IDXGISwapChain>		g_pSwapChain{ nullptr };
	MWRL::ComPtr<ID3D11Device>			g_pd3dDevice{ nullptr };
	MWRL::ComPtr<ID3D11DeviceContext>	g_pImmediateContext{ nullptr };
	MWRL::ComPtr<ID3D11RenderTargetView>	g_pRenderTargetView{ nullptr };
	MWRL::ComPtr<ID3D11Buffer>			g_pVertexBuffer{ nullptr };
	MWRL::ComPtr<ID3D11Buffer>			g_pIndexBuffer{ nullptr };
	MWRL::ComPtr<ID3D11InputLayout>			g_pVertexLayout{ nullptr };
	MWRL::ComPtr<ID3D11VertexShader>			m_VertexShader{ nullptr };
	MWRL::ComPtr<ID3D11PixelShader>			m_PixelShader{ nullptr };

private:
	// How To: Create a Device and Immediate Context
	HRESULT CreateDeviceandContext() {
		HRESULT hr;

		DXGI_SWAP_CHAIN_DESC sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.BufferCount = 1;
		sd.BufferDesc.Width = this->m_Width;
		sd.BufferDesc.Height = this->m_Height;
		sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.BufferDesc.RefreshRate.Numerator = 60;
		sd.BufferDesc.RefreshRate.Denominator = 1;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.OutputWindow = this->g_hWnd;
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
		sd.Windowed = TRUE;

		//D3D_FEATURE_LEVEL  FeatureLevelsRequested = D3D_FEATURE_LEVEL_11_0;
		//UINT               numFeatureLevelsRequested = 1;
		D3D_FEATURE_LEVEL  FeatureLevelsSupported;
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
			//D3D_FEATURE_LEVEL_11_4,
			//D3D_FEATURE_LEVEL_11_3,
			//D3D_FEATURE_LEVEL_11_2,
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

		//ID3D11Device* device = nullptr;
		hr = D3D11CreateDeviceAndSwapChain(nullptr,
			D3D_DRIVER_TYPE_HARDWARE,
			nullptr, createDeviceFlags,
			lvl, _countof(lvl),
			D3D11_SDK_VERSION,
			&sd,
			this->g_pSwapChain.ReleaseAndGetAddressOf(),
			this->g_pd3dDevice.ReleaseAndGetAddressOf(),
			&FeatureLevelsSupported,
			this->g_pImmediateContext.ReleaseAndGetAddressOf());
		if (hr == E_INVALIDARG)
		{
			hr = D3D11CreateDeviceAndSwapChain(nullptr,
				D3D_DRIVER_TYPE_HARDWARE,
				nullptr, createDeviceFlags,
				&lvl[1], _countof(lvl) - 1,
				D3D11_SDK_VERSION,
				&sd,
				this->g_pSwapChain.ReleaseAndGetAddressOf(),
				this->g_pd3dDevice.ReleaseAndGetAddressOf(),
				&FeatureLevelsSupported,
				this->g_pImmediateContext.ReleaseAndGetAddressOf());
		}
		if (FAILED(hr)) {
			return hr;
		}

		MWRL::ComPtr<ID3D11Texture2D> pBackBuffer;

		// Get a pointer to the back buffer
		hr = this->g_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)pBackBuffer.ReleaseAndGetAddressOf());

		// Create a render-target view
		this->g_pd3dDevice->CreateRenderTargetView(pBackBuffer.Get(), NULL,
			this->g_pRenderTargetView.ReleaseAndGetAddressOf());

		// Bind the view
		ID3D11RenderTargetView* targets[1];
		targets[0] = this->g_pRenderTargetView.Get();
		this->g_pImmediateContext->OMSetRenderTargets(1, targets, NULL);

		// Setup the viewport
		D3D11_VIEWPORT vp;
		vp.Width = (FLOAT)this->m_Width;
		vp.Height = (FLOAT)this->m_Height;
		vp.MinDepth = 0.0F;
		vp.MaxDepth = 1.0F;
		vp.TopLeftX = 0.0F;
		vp.TopLeftY = 0.0F;
		this->g_pImmediateContext->RSSetViewports(1, &vp);

		return hr;
	}

	HRESULT CompileShader(_In_ LPCWSTR srcFile, _In_ LPCSTR entryPoint, _In_ LPCSTR profile, _Outptr_ ID3DBlob** blob)
	{
		if (!srcFile || !entryPoint || !profile || !blob)
			return E_INVALIDARG;

		*blob = nullptr;

		UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
		flags |= D3DCOMPILE_DEBUG;
#endif

		const D3D_SHADER_MACRO defines[] =
		{
			"EXAMPLE_DEFINE", "1",
			NULL, NULL
		};

		ID3DBlob* shaderBlob = nullptr;
		MWRL::ComPtr<ID3DBlob> errorBlob = nullptr;
		HRESULT hr = D3DCompileFromFile(srcFile, defines, D3D_COMPILE_STANDARD_FILE_INCLUDE,
			entryPoint, profile,
			flags, 0, &shaderBlob, &errorBlob);
		if (FAILED(hr))
		{
			if (errorBlob)
			{
				OutputDebugStringA((char*)errorBlob->GetBufferPointer());
			}

			if (shaderBlob)
				shaderBlob->Release();

			return hr;
		}

		*blob = shaderBlob;

		return hr;
	}

	HRESULT CreateResources() {
		HRESULT hr;

		// Supply the actual vertex data.
		SimpleVertexCombined verticesCombo[] =
		{
			XMFLOAT3(0.0f, 0.5f, 0.5f), XMFLOAT3(0.0f, 0.0f, 0.5f),
			XMFLOAT3(0.5f, -0.5f, 0.5f), XMFLOAT3(0.5f, 0.0f, 0.0f),
			XMFLOAT3(-0.5f, -0.5f, 0.5f), XMFLOAT3(0.0f, 0.5f, 0.0f),
		};

		// Fill in a buffer description.
		D3D11_BUFFER_DESC bufferDesc;
		bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		bufferDesc.ByteWidth = sizeof(verticesCombo);
		bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bufferDesc.CPUAccessFlags = 0;
		bufferDesc.MiscFlags = 0;

		// Fill in the subresource data.
		D3D11_SUBRESOURCE_DATA InitData;
		InitData.pSysMem = verticesCombo;
		InitData.SysMemPitch = 0;
		InitData.SysMemSlicePitch = 0;

		// Create the vertex buffer.
		hr = this->g_pd3dDevice->CreateBuffer(&bufferDesc, &InitData, this->g_pVertexBuffer.ReleaseAndGetAddressOf());

		// Create indices.
		unsigned short indices[] = { 0, 1, 2 };

		// Fill in a buffer description.
		bufferDesc.ByteWidth = sizeof(indices);
		bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

		// Define the resource data.
		InitData.pSysMem = indices;

		// Create the buffer with the device.
		hr = this->g_pd3dDevice->CreateBuffer(&bufferDesc, &InitData, this->g_pIndexBuffer.ReleaseAndGetAddressOf());
		if (FAILED(hr))
			return hr;

		UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
		flags |= D3DCOMPILE_DEBUG;
#endif
		// Prefer higher CS shader profile when possible as CS 5.0 provides better performance on 11-class hardware.
		const D3D_SHADER_MACRO defines[] =
		{
			"EXAMPLE_DEFINE", "1",
			NULL, NULL
		};
		ID3DBlob* errorBlob = nullptr;
		LPCSTR profile = (this->g_pd3dDevice->GetFeatureLevel() >= D3D_FEATURE_LEVEL_11_0) ? "vs_5_0" : "vs_4_0";
		MWRL::ComPtr<ID3DBlob> VSshaderBlob = nullptr;
		hr = this->CompileShader(L"VertexShader.hlsl", "main", profile, VSshaderBlob.ReleaseAndGetAddressOf());
		profile = (this->g_pd3dDevice->GetFeatureLevel() >= D3D_FEATURE_LEVEL_11_0) ? "ps_5_0" : "ps_4_0";
		MWRL::ComPtr<ID3DBlob> PSshaderBlob = nullptr;
		hr = this->CompileShader(L"PixelShader.hlsl", "main", profile, PSshaderBlob.ReleaseAndGetAddressOf());

		hr = this->g_pd3dDevice->CreateVertexShader(VSshaderBlob->GetBufferPointer(), VSshaderBlob->GetBufferSize(),
			nullptr, this->m_VertexShader.ReleaseAndGetAddressOf());
		hr = this->g_pd3dDevice->CreatePixelShader(PSshaderBlob->GetBufferPointer(), PSshaderBlob->GetBufferSize(),
			nullptr, this->m_PixelShader.ReleaseAndGetAddressOf());

		hr = this->g_pd3dDevice->CreateInputLayout(this->layout, 2, VSshaderBlob->GetBufferPointer(), VSshaderBlob->GetBufferSize(),
			this->g_pVertexLayout.ReleaseAndGetAddressOf());

		return hr;
	}

public:
	Dx11Engine() {
	};

	~Dx11Engine() {};


	HRESULT Initialize(HWND hWnd) {
		this->g_hWnd = hWnd;

		HRESULT hr = this->CreateDeviceandContext();
		if (FAILED(hr)) {
			return hr;
		}
		return this->CreateResources();
	}

	void Render() {
		FLOAT bkClr[4] = { 0.0F, 0.0F, 1.0F, 1.0F };
		this->g_pImmediateContext->ClearRenderTargetView(this->g_pRenderTargetView.Get(), bkClr);

		UINT stride = sizeof(SimpleVertexCombined);
		UINT offset = 0;
		this->g_pImmediateContext->IASetVertexBuffers(0, 1, this->g_pVertexBuffer.GetAddressOf(), &stride, &offset);
		// Set the input layout
		this->g_pImmediateContext->IASetInputLayout(this->g_pVertexLayout.Get());
		// Set the buffer.
		this->g_pImmediateContext->IASetIndexBuffer(this->g_pIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);
		this->g_pImmediateContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		this->g_pImmediateContext->VSSetShader(this->m_VertexShader.Get(), nullptr, 0);

		this->g_pImmediateContext->PSSetShader(this->m_PixelShader.Get(), nullptr, 0);

		this->g_pImmediateContext->DrawIndexed(3, 0, 0);

		this->g_pSwapChain->Present(0, 0);
	}
};

#pragma once
#include "Dx11Shader.h"

using namespace DirectX;
using namespace Microsoft::WRL;

class Dx11Model
{
public:
	Dx11Model() {};
	~Dx11Model() {};

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

	ComPtr<ID3D11Buffer>			m_pVertexBuffer{ nullptr };
	ComPtr<ID3D11Buffer>			m_pIndexBuffer{ nullptr };
	ComPtr<ID3D11InputLayout>			m_pVertexLayout{ nullptr };
	ComPtr<ID3D11VertexShader>			m_VertexShader{ nullptr };
	ComPtr<ID3D11PixelShader>			m_PixelShader{ nullptr };
	ComPtr<ID3D11Buffer>	m_ObjConstantBuffer{ nullptr };
	XMMATRIX m_WorldMatrix;
	UINT m_IndexCount{ 0 };

public:
	HRESULT Initialize(ID3D11Device* pd3dDevice) {
		HRESULT hr = S_OK;

		SimpleVertexCombined verticesCombo[8]
		{
			XMFLOAT3(-0.5f, -0.5f, -0.5f), XMFLOAT3(0.0f, 0.0f, 0.5f),
			XMFLOAT3(-0.5f, 0.5f, -0.5f), XMFLOAT3(0.5f, 0.0f, 0.0f),
			XMFLOAT3(0.5f, 0.5f, -0.5f), XMFLOAT3(0.0f, 0.5f, 0.0f),
			XMFLOAT3(0.5f, -0.5f, -0.5f), XMFLOAT3(0.0f, 0.5f, 0.0f),
			XMFLOAT3(-0.5f, -0.5f, 0.5f), XMFLOAT3(0.0f, 0.0f, 0.5f),
			XMFLOAT3(-0.5f, 0.5f, 0.5f), XMFLOAT3(0.5f, 0.0f, 0.0f),
			XMFLOAT3(0.5f, 0.5f, 0.5f), XMFLOAT3(0.0f, 0.5f, 0.0f),
			XMFLOAT3(0.5f, -0.5f, 0.5f), XMFLOAT3(0.0f, 0.5f, 0.0f),
		};

		// Supply the actual vertex data.
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
		hr = pd3dDevice->CreateBuffer(&bufferDesc, &InitData, this->m_pVertexBuffer.ReleaseAndGetAddressOf());

		// Create indices.
		unsigned short indices[] = {
			7, 6, 1, 7, 1, 0,
			0, 1, 2, 0, 2, 3,
			3, 2, 5, 3, 5, 4,
			1, 6, 5, 1, 5, 2,
			4, 5, 6, 4, 6, 7,
			7, 0, 3, 7, 3, 4
		};
		m_IndexCount = 36;

		// Fill in a buffer description.
		bufferDesc.ByteWidth = sizeof(indices);
		bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

		// Define the resource data.
		InitData.pSysMem = indices;

		// Create the buffer with the device.
		hr = pd3dDevice->CreateBuffer(&bufferDesc, &InitData, this->m_pIndexBuffer.ReleaseAndGetAddressOf());
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
		LPCSTR profile = (pd3dDevice->GetFeatureLevel() >= D3D_FEATURE_LEVEL_11_0) ? "vs_5_0" : "vs_4_0";
		ComPtr<ID3DBlob> VSshaderBlob = nullptr;
		hr = CompileShader(L"VertexShader.hlsl", "main", profile, VSshaderBlob.ReleaseAndGetAddressOf());
		profile = (pd3dDevice->GetFeatureLevel() >= D3D_FEATURE_LEVEL_11_0) ? "ps_5_0" : "ps_4_0";
		ComPtr<ID3DBlob> PSshaderBlob = nullptr;
		hr = CompileShader(L"PixelShader.hlsl", "main", profile, PSshaderBlob.ReleaseAndGetAddressOf());

		hr = pd3dDevice->CreateVertexShader(VSshaderBlob->GetBufferPointer(), VSshaderBlob->GetBufferSize(),
			nullptr, this->m_VertexShader.ReleaseAndGetAddressOf());
		hr = pd3dDevice->CreatePixelShader(PSshaderBlob->GetBufferPointer(), PSshaderBlob->GetBufferSize(),
			nullptr, this->m_PixelShader.ReleaseAndGetAddressOf());

		hr = pd3dDevice->CreateInputLayout(this->layout, 2, VSshaderBlob->GetBufferPointer(), VSshaderBlob->GetBufferSize(),
			this->m_pVertexLayout.ReleaseAndGetAddressOf());

		return hr;
	}

	void Update(float fTime) {

	}

	void Render(ID3D11DeviceContext* pd3dDeviceContext, const XMMATRIX& viewMatrix, const XMMATRIX& projMatrix) const {
		UINT stride[1] = { sizeof(SimpleVertexCombined) };
		UINT offset[1] = { 0 };
		pd3dDeviceContext->IASetVertexBuffers(0, 1, this->m_pVertexBuffer.GetAddressOf(), stride, offset);
		// Set the input layout
		pd3dDeviceContext->IASetInputLayout(this->m_pVertexLayout.Get());
		// Set the buffer.
		pd3dDeviceContext->IASetIndexBuffer(this->m_pIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);
		pd3dDeviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		pd3dDeviceContext->VSSetShader(this->m_VertexShader.Get(), nullptr, 0);

		pd3dDeviceContext->PSSetShader(this->m_PixelShader.Get(), nullptr, 0);

		pd3dDeviceContext->DrawIndexed(m_IndexCount, 0, 0);
	}
};

#pragma once
#include "Dx11Shader.h"
#include "WICTextureLoader.h"

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
	struct PosColorVertexType
	{
		XMFLOAT3 Pos;
		XMFLOAT3 Color;
	};
	struct PosNormTexVertexType {
		XMFLOAT3 Position;
		XMFLOAT3 Normal;
		XMFLOAT2 Tex0;
	};
	// Per instance data, 16 bytes aligned
	__declspec(align(16)) struct PlaneInstanceData {
		XMMATRIX matWorld;
		XMMATRIX matInverseWorld;
	};

	D3D11_INPUT_ELEMENT_DESC layout[2]{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	//{ L"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
	//{ L"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	struct PerObjectConstantData {
		XMMATRIX matWorld;
		XMMATRIX matInverseWorld;
		XMMATRIX matWorldViewProjection;
	};

	ComPtr<ID3D11DeviceContext>		m_Context{ nullptr };
	ComPtr<ID3D11Buffer>			m_pVertexBuffer{ nullptr };
	ComPtr<ID3D11Buffer>			m_pIndexBuffer{ nullptr };
	ComPtr<ID3D11InputLayout>		m_pVertexLayout{ nullptr };
	ComPtr<ID3D11VertexShader>		m_VertexShader{ nullptr };
	ComPtr<ID3D11PixelShader>		m_PixelShader{ nullptr };
	ComPtr<ID3D11PixelShader>		m_PixelShaderFrame{ nullptr };
	ComPtr<ID3D11Buffer>	m_ObjConstantBuffer{ nullptr };
	ComPtr<ID3D11ShaderResourceView>	m_Texture{ nullptr };
	ComPtr<ID3D11SamplerState>		m_SamplerState{ nullptr };
	UINT m_IndexCount{ 0 };

public:
	HRESULT Initialize(ID3D11Device* pd3dDevice) {
		HRESULT hr = S_OK;
		pd3dDevice->GetImmediateContext(m_Context.ReleaseAndGetAddressOf());

		//PosColorVertexType verticesCombo[8]
		//{
		//	XMFLOAT3(-0.5f, -0.5f, -0.5f), XMFLOAT3(0.0f, 0.0f, 0.5f),
		//	XMFLOAT3(-0.5f, 0.5f, -0.5f), XMFLOAT3(0.5f, 0.0f, 0.0f),
		//	XMFLOAT3(0.5f, 0.5f, -0.5f), XMFLOAT3(0.0f, 0.5f, 0.0f),
		//	XMFLOAT3(0.5f, -0.5f, -0.5f), XMFLOAT3(0.0f, 0.5f, 0.0f),
		//	XMFLOAT3(-0.5f, -0.5f, 0.5f), XMFLOAT3(0.0f, 0.0f, 0.5f),
		//	XMFLOAT3(-0.5f, 0.5f, 0.5f), XMFLOAT3(0.5f, 0.0f, 0.0f),
		//	XMFLOAT3(0.5f, 0.5f, 0.5f), XMFLOAT3(0.0f, 0.5f, 0.0f),
		//	XMFLOAT3(0.5f, -0.5f, 0.5f), XMFLOAT3(0.0f, 0.5f, 0.0f),
		//};
		PosNormTexVertexType planeVerts[4] = {
			{ XMFLOAT3(-0.5f, 0.0f,  0.5f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) }, // 0
		{ XMFLOAT3(0.5f, 0.0f,  0.5f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) }, // 1
		{ XMFLOAT3(0.5f, 0.0f, -0.5f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) }, // 2
		{ XMFLOAT3(-0.5f, 0.0f, -0.5f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) }  // 3
		};

		// Supply the actual vertex data.
		// Fill in a buffer description.
		D3D11_BUFFER_DESC bufferDesc;
		bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		bufferDesc.ByteWidth = sizeof(planeVerts);
		bufferDesc.CPUAccessFlags = 0;
		bufferDesc.MiscFlags = 0;

		// Fill in the subresource data.
		D3D11_SUBRESOURCE_DATA InitData;
		InitData.pSysMem = planeVerts;
		InitData.SysMemPitch = 0;
		InitData.SysMemSlicePitch = 0;

		// Create the vertex buffer.
		hr = pd3dDevice->CreateBuffer(&bufferDesc, &InitData, this->m_pVertexBuffer.ReleaseAndGetAddressOf());

		// Create indices.
		//unsigned short indices[] = {
		//	4, 5, 1, 4, 1, 0,
		//	0, 1, 2, 0, 2, 3,
		//	3, 2, 6, 3, 6, 7,
		//	1, 5, 6, 1, 6, 2,
		//	5, 4, 7, 5, 7, 6,
		//	4, 0, 3, 4, 3, 7
		//};
		unsigned short planeIndices[6] = {
			0, 1, 3, 1, 2, 3
		};
		m_IndexCount = 6;

		// Fill in a buffer description.
		bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bufferDesc.ByteWidth = sizeof(planeIndices);

		// Define the resource data.
		InitData.pSysMem = planeIndices;

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
		ComPtr<ID3DBlob> shaderBlob = nullptr;
		{
			hr = CompileShader(L"VertexShader.hlsl", "main", profile, shaderBlob.ReleaseAndGetAddressOf());
			if (FAILED(hr)) return hr;

			hr = pd3dDevice->CreateVertexShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(),
				nullptr, this->m_VertexShader.ReleaseAndGetAddressOf());
			if (FAILED(hr)) return hr;

			hr = pd3dDevice->CreateInputLayout(this->layout, 2, shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(),
				this->m_pVertexLayout.ReleaseAndGetAddressOf());
			if (FAILED(hr)) return hr;
		}

		{
			profile = (pd3dDevice->GetFeatureLevel() >= D3D_FEATURE_LEVEL_11_0) ? "ps_5_0" : "ps_4_0";
			hr = CompileShader(L"PixelShader.hlsl", "main", profile, shaderBlob.ReleaseAndGetAddressOf());
			if (FAILED(hr)) return hr;

			hr = pd3dDevice->CreatePixelShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(),
				nullptr, this->m_PixelShader.ReleaseAndGetAddressOf());
			if (FAILED(hr)) return hr;

			hr = CompileShader(L"PixelShader.hlsl", "wireFrame", profile, shaderBlob.ReleaseAndGetAddressOf());
			if (FAILED(hr)) return hr;

			hr = pd3dDevice->CreatePixelShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(),
				nullptr, this->m_PixelShaderFrame.ReleaseAndGetAddressOf());
			if (FAILED(hr)) return hr;
		}

		{
			// Create the constant buffers for the object's world
			bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			bufferDesc.ByteWidth = sizeof(PerObjectConstantData);
			hr = pd3dDevice->CreateBuffer(&bufferDesc, nullptr, this->m_ObjConstantBuffer.ReleaseAndGetAddressOf());
			if (FAILED(hr)) return hr;
		}

		hr = CreateWICTextureFromFile(pd3dDevice, L"E:/Games/MineCraft/assets/minecraft/textures/blocks/emerald_ore.png", nullptr, m_Texture.ReleaseAndGetAddressOf());
		if (FAILED(hr)) return hr;

		{
			D3D11_SAMPLER_DESC samplerDesc;
			ZeroMemory(&samplerDesc, sizeof(samplerDesc));
			samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
			samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
			samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
			samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
			samplerDesc.MipLODBias = 0.0f;
			samplerDesc.MaxAnisotropy = 1;
			samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
			samplerDesc.BorderColor[0] = 1.0f;
			samplerDesc.BorderColor[1] = 1.0f;
			samplerDesc.BorderColor[2] = 1.0f;
			samplerDesc.BorderColor[3] = 1.0f;
			samplerDesc.MinLOD = -FLT_MAX;
			samplerDesc.MaxLOD = FLT_MAX;

			hr = pd3dDevice->CreateSamplerState(&samplerDesc, m_SamplerState.ReleaseAndGetAddressOf());
			if (FAILED(hr)) return hr;
		}

		return hr;
	}

	//const XMMATRIX* World() const { return &m_WorldMatrix; }
	ID3D11Buffer* Constants() const { return m_ObjConstantBuffer.Get(); }

	void Update(float fTime) {
		static float angle = 0.0f;
		angle += 90.0f * fTime;
		//XMVECTOR rotation = XMVectorSet(0, 1, 1, 0);
		PerObjectConstantData constData;
		constData.matWorld = XMMatrixRotationY(XMConvertToRadians(angle));
		this->m_Context->UpdateSubresource(m_ObjConstantBuffer.Get(), 0, nullptr, &constData, 0, 0);
	}

	void Render(ID3D11DeviceContext* pd3dDeviceContext, const XMMATRIX& viewMatrix, const XMMATRIX& projMatrix, bool wireFrame = false) const {
		UINT stride[1] = { sizeof(PosColorVertexType) };
		UINT offset[1] = { 0 };
		pd3dDeviceContext->IASetVertexBuffers(0, 1, this->m_pVertexBuffer.GetAddressOf(), stride, offset);
		// Set the input layout
		pd3dDeviceContext->IASetInputLayout(this->m_pVertexLayout.Get());
		// Set the buffer.
		pd3dDeviceContext->IASetIndexBuffer(this->m_pIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);
		pd3dDeviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		pd3dDeviceContext->VSSetShader(this->m_VertexShader.Get(), nullptr, 0);
		pd3dDeviceContext->VSSetConstantBuffers(2, 1, m_ObjConstantBuffer.GetAddressOf());

		if (wireFrame) {
			pd3dDeviceContext->PSSetShader(this->m_PixelShaderFrame.Get(), nullptr, 0);
		}
		else {
			pd3dDeviceContext->PSSetShader(this->m_PixelShader.Get(), nullptr, 0);
		}

		pd3dDeviceContext->DrawIndexed(m_IndexCount, 0, 0);
	}
};

#include "pch.h"
#include "DxColorShader.h"
#include "DXHelper.h"

DxColorShader::DxColorShader()
{
}


DxColorShader::~DxColorShader()
{
}

bool DxColorShader::Initialize(ID3D11Device * pDevice, HWND hWnd)
{
	if (!InitializeShader(pDevice, hWnd)) {
		return false;
	}

	return true;
}

void DxColorShader::Destroy()
{
	DestroyShader();
}

bool DxColorShader::Render(ID3D11DeviceContext * pContext, int indexCount, XMMATRIX world, XMMATRIX view, XMMATRIX projection)
{
	if (!SetShaderParameters(pContext, world, view, projection)) {
		return false;
	}

	RenderShader(pContext, indexCount);

	return true;
}

bool DxColorShader::InitializeShader(ID3D11Device * pDevice, HWND hWnd)
{
	// Compile the shaders.

	// Define the vertex input layout.
	D3D11_INPUT_ELEMENT_DESC inputElementDescs[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	if (!CompileVertexShaderFromFile(pDevice, DX::GetFullPathShaders(L"vertexcolor.hlsl").c_str(), "ColorVertexShader", 0, &m_VertexShader,
		inputElementDescs, ARRAYSIZE(inputElementDescs),  &m_InputLayout)) {
		return false;
	}

	if (!CompilePixelShaderFromFile(pDevice, DX::GetFullPathShaders(L"pixelcolor.hlsl").c_str(), "ColorPixelShader", 0, &m_PixelShader)) {
		return false;
	}

	// Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
	D3D11_BUFFER_DESC matrixBufferDesc;
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;
	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	HRESULT hr = pDevice->CreateBuffer(&matrixBufferDesc, NULL, &m_MatrixBuffer);
	if (FAILED(hr))
	{
		OutputDebugStringW(L"Create Matrix Buffer failed.\n");
		return false;
	}

	return true;
}

void DxColorShader::DestroyShader()
{
	// todo:
}

bool DxColorShader::SetShaderParameters(ID3D11DeviceContext * pContext, XMMATRIX world, XMMATRIX view, XMMATRIX projection)
{
	world = XMMatrixTranspose(world);
	view = XMMatrixTranspose(view);
	projection = XMMatrixTranspose(projection);

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	HRESULT hr = pContext->Map(m_MatrixBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(hr)) {
		OutputDebugStringW(L"Map Matrix Buffer failed.\n");
		return false;
	}

	MatrixBufferType* pBuffer = (MatrixBufferType*)mappedResource.pData;
	pBuffer->world = world;
	pBuffer->view = view;
	pBuffer->projection = projection;

	pContext->Unmap(m_MatrixBuffer.Get(), 0);

	pContext->VSSetConstantBuffers(0, 1, m_MatrixBuffer.GetAddressOf());

	return true;
}

void DxColorShader::RenderShader(ID3D11DeviceContext * pContext, int indexCount)
{
	pContext->IASetInputLayout(m_InputLayout.Get());
	pContext->VSSetShader(m_VertexShader.Get(), nullptr, 0);
	pContext->PSSetShader(m_PixelShader.Get(), nullptr, 0);

	pContext->DrawIndexed(indexCount, 0, 0);
}

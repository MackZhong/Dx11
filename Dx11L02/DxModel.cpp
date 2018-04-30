#include "pch.h"
#include "DxModel.h"


DxModel::DxModel()
{
}


DxModel::~DxModel()
{
}

void DxModel::Resize(float aspectRatio)
{
	// todo: wait
}

bool DxModel::Initialize(ID3D11Device * pDevice, float aspectRatio)
{
	/************************************************************************/
	/*  Initialize the assets.                                              */
	/************************************************************************/

	// Create the vertex buffer.
	// Define the geometry for a triangle.
	VertexPC triangleVertices[] =
	{
		{ { -1.0F * aspectRatio,-1.0f, 0.0F },{ 1.0F, 0.0F, 0.0F, 1.0F } },
	{ { 0.0F * aspectRatio, 1.0f, 0.0F },{ 0.0F, 1.0F, 0.0F, 1.0F } },
	{ { 1.0F * aspectRatio, -1.0f, 0.0F },{ 0.0F, 0.0F, 1.0F, 1.0F } }
	};
	//{
	//	{ { 0.0F, 0.25F * aspectRatio, 0.0F },{ 1.0F, 0.0F, 0.0F, 1.0F } },
	//{ { 0.25F, -0.25F * aspectRatio, 0.0F },{ 0.0F, 1.0F, 0.0F, 1.0F } },
	//{ { -0.25F, -0.25F * aspectRatio, 0.0F },{ 0.0F, 0.0F, 1.0F, 1.0F } }
	//};
	m_VertexCount = 3;

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
	// Set up the description of the static vertex buffer.
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(triangleVertices);
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	bd.StructureByteStride = 0;
	// Give the subresource structure a pointer to the vertex data.
	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(D3D11_SUBRESOURCE_DATA));
	sd.pSysMem = triangleVertices;
	sd.SysMemPitch = 0;
	sd.SysMemSlicePitch = 0;
	HRESULT hr = pDevice->CreateBuffer(&bd, &sd, &m_VertexBuffer);
	if (FAILED(hr))
	{
		OutputDebugStringW(L"Create Vertex Buffer failed.\n");
		return false;
	}

	// 创建索引缓冲区
	DWORD indices[] = { 0, 1, 2 };
	m_IndexCount = 3;

	bd.ByteWidth = sizeof(indices);
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	sd.pSysMem = indices;
	hr = pDevice->CreateBuffer(&bd, &sd, &m_IndexBuffer);
	if (FAILED(hr))
	{
		OutputDebugStringW(L"Create Index Buffer failed.\n");
		return false;
	}

	return true;
}

void DxModel::Destroy()
{
	// nothing to do
}

void DxModel::Render(ID3D11DeviceContext * pContext)
{
	UINT stride = sizeof(VertexPC);
	UINT offset = 0;
	pContext->IASetVertexBuffers(0, 1, m_VertexBuffer.GetAddressOf(), &stride, &offset);
	pContext->IASetIndexBuffer(m_IndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	pContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

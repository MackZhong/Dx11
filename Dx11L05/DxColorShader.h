#pragma once
using namespace DirectX;
using namespace Microsoft::WRL;

class DxColorShader
{
private:
	struct MatrixBufferType
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
	};
	ComPtr<ID3D11VertexShader> m_VertexShader{ nullptr };
	ComPtr<ID3D11PixelShader> m_PixelShader{ 0 };
	ComPtr<ID3D11InputLayout> m_InputLayout{ 0 };
	ComPtr<ID3D11Buffer> m_MatrixBuffer{ 0 };

public:
	DxColorShader();
	~DxColorShader();

public:
	bool Initialize(ID3D11Device* pDevice, HWND hWnd);
	void Destroy();
	bool Render(ID3D11DeviceContext* pContext, int indexCount, XMMATRIX world, XMMATRIX view, XMMATRIX projection);

private:
	bool InitializeShader(ID3D11Device* pDevice, HWND hWnd);
	void DestroyShader();
	bool SetShaderParameters(ID3D11DeviceContext* pContext, XMMATRIX world, XMMATRIX view, XMMATRIX projection);
	void RenderShader(ID3D11DeviceContext* pContext, int indexCount);
};


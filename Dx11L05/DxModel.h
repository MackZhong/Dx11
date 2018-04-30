#pragma once
using namespace DirectX;
using namespace Microsoft::WRL;

class DxModel
{
private:
	struct VertexPC
	{
		XMFLOAT3 position;
		XMFLOAT4 color;
	};

	ComPtr<ID3D11Buffer> m_VertexBuffer{ nullptr };
	UINT m_VertexCount{ 0 };
	ComPtr<ID3D11Buffer> m_IndexBuffer{ nullptr };
	UINT m_IndexCount{ 0 };

public:
	DxModel();
	~DxModel();

	void Resize(float aspectRatio);
public:
	UINT VertexCount() { return m_VertexCount; }
	UINT IndexCount() { return m_IndexCount; }

public:
	bool Initialize(ID3D11Device* pDevice, float aspectRatio);
	void Destroy();
	void Render(ID3D11DeviceContext* pContext);
};


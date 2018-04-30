#pragma once
//#include <d3d12sdklayers.h>

using namespace std;
using namespace DirectX;
using namespace Microsoft::WRL;

//#define D3DCOMPILE_DEBUG 1

class D3D11Engine
{
private:
	struct Vertex
	{
		XMFLOAT3 position;
		XMFLOAT4 color;
	};

	//static const UINT FrameCount = 2;
	std::wstring m_AssetsPath;
	float m_AspectRatio{ 1.0f };
	float m_FOV{ 3.141592654f / 4.0f };
	HWND m_hWnd{ nullptr };
	UINT m_Width{ 800 };
	UINT m_Height{ 600 };
	D3D11_VIEWPORT m_Viewport;
	XMMATRIX m_matProjection;
	XMMATRIX m_matWorld;
	XMMATRIX m_matOrtho;
	bool m_Warp{ false };
	bool m_bVsync{ true };
	bool m_bFullScreen{ false };
	int m_VideoMemory{ 0 };
	std::wstring m_VideoCard;
	DXGI_FORMAT m_BufferFormat{ DXGI_FORMAT_R8G8B8A8_UNORM };
	DXGI_FORMAT m_DepthStencilFormat{ DXGI_FORMAT_D24_UNORM_S8_UINT };
	ComPtr<IDXGISwapChain> m_SwapChain{ nullptr };
	ComPtr<ID3D11Device> m_Device{ nullptr };
	ComPtr<ID3D11DeviceContext> m_Context{ nullptr };
	ComPtr<ID3D11RenderTargetView> m_RenderTargetView{ nullptr };
	ComPtr<ID3D11DepthStencilState> m_DepthStencilState{ nullptr };
	ComPtr<ID3D11DepthStencilView> m_DepthStencilView{ nullptr };
	ComPtr<ID3D11RasterizerState> m_RasterizerState{ nullptr };

	// App resources.
	ComPtr<ID3D11Buffer> m_VertexBuffer{ nullptr };
	UINT m_VertexCount{ 0 };
	ComPtr<ID3D11Buffer> m_IndexBuffer{ nullptr };
	UINT m_IndexCount{ 0 };
	ComPtr<ID3D11InputLayout> m_InputLayout{ nullptr };
	ComPtr<ID3D11VertexShader> m_VertexShader{ nullptr };
	ComPtr<ID3D11PixelShader> m_PixelShader{ nullptr };

	// Synchronization object.
	UINT m_FrameIndex{ 0 };
	//HANDLE m_FenceEvent;
	//ComPtr<ID3D11Fence> m_Fence{ nullptr };
	//UINT64 m_FenceValue{ 0 };

	//D3D11_RECT m_ScissorRect;

public:
	D3D11Engine();
	~D3D11Engine();

	void BeginScene(float cr, float cg, float cb, float ca);
	void Render();
	void EndScene();

private:
	std::wstring GetAssetFullPath(LPCWSTR assetName);
	HRESULT GetHardwareAdapter(IDXGIFactory4* pFactory, IDXGIAdapter1** ppAdapter, IDXGIOutput** ppOutput, DXGI_MODE_DESC* pModeDesc);
	bool LoadPipeline();
	bool LoadAssets();
	//void PopulateCommandList();
	//void WaitforPreviousFrame();

public:
	ID3D11Device * Device() { return m_Device.Get(); }
	bool Initialize(HWND hWnd, int Width, int Height, float ScreenDepth, float ScreenNear, bool vSync, bool fullscreen);
	void Destroy();
	bool UpdateFrame();
};

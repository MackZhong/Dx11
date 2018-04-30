#pragma once
using namespace DirectX;
class DxCamera
{
private:
	XMVECTOR m_Position{ 0.0f, 0.0f, 0.0f, 0.0f };
	XMFLOAT3 m_Rotation{ 0.0f, 0.0f, 0.0f };
	XMMATRIX m_matView;


public:
	DxCamera();
	~DxCamera();

public:
	XMFLOAT3 GetPosition() { XMFLOAT3 pos; XMStoreFloat3(&pos, m_Position); return pos; }
	void SetPostiion(float x, float y, float z) { m_Position = { x, y, z, 0.0f }; }
	XMFLOAT3 GetRotation() { return m_Rotation; }
	void SetRotation(float rx, float ry, float rz) { m_Rotation = { rx, ry, rz }; }
	void GetViewMatrix(XMMATRIX& matView) { matView = m_matView; }
	void Render();
};


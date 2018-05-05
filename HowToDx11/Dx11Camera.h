#pragma once

using namespace DirectX;
using namespace Microsoft::WRL;

class Dx11Camera
{
private:
	XMMATRIX m_ViewMatrix;
	XMMATRIX m_ProjectionMatrix;
	XMVECTOR _Up{ 0, 1, 0, 0 };

public:
	Dx11Camera() {};

public:
	XMMATRIX & View() { return m_ViewMatrix; };
	XMMATRIX& Projection() { return m_ProjectionMatrix; };
	XMMATRIX& SetProjection(float width, float height) {
		m_ProjectionMatrix = XMMatrixPerspectiveFovLH(XMConvertToRadians(70.0f), width / height, 0.1f, 1000.0f);

		return m_ProjectionMatrix;
	}
	XMMATRIX& SetView(const XMVECTOR& pos, const XMVECTOR& lookAt) {
		m_ViewMatrix = XMMatrixLookAtLH(pos, lookAt, _Up);

		return m_ViewMatrix;
	}
	void Update(float fTime) {
		SetView(XMVectorSet(0, 5, -5, 1), XMVectorSet(0, 0, 0, 1));
	}

};

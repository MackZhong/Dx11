#pragma once

using namespace DirectX;
using namespace Microsoft::WRL;

class Dx11Camera
{
private:
	XMMATRIX m_ViewMatrix;

public:
	Dx11Camera() {};
public:

	XMMATRIX & View() { return m_ViewMatrix; };
};

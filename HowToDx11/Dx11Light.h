#pragma once

class Dx11Light
{
public:
	typedef enum _Type{
		Point, Spot, Directional
	}Type;

private:
	Type m_Type;
	DirectX::XMFLOAT3	m_Color;
	DirectX::XMFLOAT3	m_Position;
	DirectX::XMFLOAT3	m_Direction;
	float	m_SpotAngle;
	float	m_ConstantAttenuation{ 1 };
	float	m_LinearAttenuation{ 0.2f };
	float	m_QuadraticAttenuation{ 0.1f };

public:
	Dx11Light() {};
};
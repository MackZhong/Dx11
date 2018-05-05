#pragma once

class Dx11Light
{
public:
	typedef enum _Type{
		Point, Spot, Directional
	}Type;

private:
	DirectX::XMFLOAT4	m_Position;
	DirectX::XMFLOAT4	m_Direction;
	DirectX::XMFLOAT4	m_Color;
	float	m_SpotAngle;
	float	m_ConstantAttenuation{ 1 };
	float	m_LinearAttenuation{ 0.2f };
	float	m_QuadraticAttenuation{ 0.1f };
	Type m_Type{ Point };
	bool m_Enabled{ true };

public:
	Dx11Light() {};
};
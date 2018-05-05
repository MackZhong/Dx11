#pragma once
class Dx11Material
{
private:
	float m_Emissive;
	float m_Ambient;
	float m_Diffuse;
	float m_Specular{ 0 };
	float m_SpecularPower{ 64 };

public:
	Dx11Material() {};
};

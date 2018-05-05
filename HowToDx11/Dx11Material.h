#pragma once

class Dx11Material
{
private:
	XMFLOAT4 m_Emissive{ 0.0f, 0.0f, 0.0f, 1.0f };
	XMFLOAT4 m_Ambient{ 0.1f, 0.1f, 0.1f, 1.0f };
	XMFLOAT4 m_Diffuse{ 1.0f, 1.0f, 1.0f, 1.0f };
	XMFLOAT4 m_Specular{ 1.0f, 1.0f, 1.0f, 1.0f };
	float	m_SpecularPower{ 64.0f };
	int	m_UseTexture{ false };
	float padding[2];

public:
	Dx11Material() {};
};

struct MaterialProperties
{
	MaterialProperties() {};
	MaterialProperties(const XMFLOAT4& ambient, const XMFLOAT4& diffuse, const XMFLOAT4& specular, float specularPower) {
	};
	MaterialProperties(const XMFLOAT4& diffuse, const XMFLOAT4& specular, float specularPower) {
	};
	Dx11Material   material;
};

// Create some materials
XMGLOBALCONST MaterialProperties defaultMaterial = MaterialProperties();

//m_MaterialProperties.push_back(defaultMaterial);

XMGLOBALCONST MaterialProperties greenMaterial = MaterialProperties(
	XMFLOAT4(0.07568f, 0.61424f, 0.07568f, 1.0f),
	XMFLOAT4(0.07568f, 0.61424f, 0.07568f, 1.0f),
	XMFLOAT4(0.07568f, 0.61424f, 0.07568f, 1.0f),
	76.8f
);

XMGLOBALCONST MaterialProperties redPlasticMaterial = MaterialProperties(
	XMFLOAT4(0.6f, 0.1f, 0.1f, 1.0f),
	XMFLOAT4(1.0f, 0.2f, 0.2f, 1.0f),
	32.0f);

XMGLOBALCONST MaterialProperties pearlMaterial = MaterialProperties(
	XMFLOAT4(0.25f, 0.20725f, 0.20725f, 1.0f),
	XMFLOAT4(1.0f, 0.829f, 0.829f, 1.0f),
	XMFLOAT4(0.296648f, 0.296648f, 0.296648f, 1.0f),
	11.264f);

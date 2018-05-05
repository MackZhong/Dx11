#pragma once
#include <DirectXMath.h>
using namespace DirectX;
#include <dinput.h>
#pragma comment(lib, "dinput8.lib")
#include <wrl.h>
namespace MWRL = Microsoft::WRL;

class Camera
{
private:
	bool m_Free{ true };
	const float PitchYawRatio = 0.01f;
	const float WalkSpeed = 0.01f;
	float m_yaw{ 0.0f };
	float m_pitch{ 0.0f };
	//XMFLOAT3 Up{ 0.0f, 0.0f, 0.0f };
	XMFLOAT3 _Up{ 0.0f, 1.0f, 0.0f };
	XMFLOAT3 _Right{ 1.0f,0.0f, 0.0f };
	XMFLOAT3 _LookTo{ 0.0f, 0.0f, 1.0f };
	XMFLOAT3 _Pos{ 0.0f, 10.0f, -10.f };
	XMMATRIX m_Projection;
	XMMATRIX m_View;
	MWRL::ComPtr<IDirectInputDevice8W> m_Keyboard;
	MWRL::ComPtr<IDirectInputDevice8W> m_Mouse;
	MWRL::ComPtr<IDirectInput8W> m_Input;
	DIMOUSESTATE2 m_MouseLastState;

public:
	Camera() {};
	~Camera() {};
	XMFLOAT3 Pos() const { return _Pos; }
	void Pos(const XMFLOAT3& pos) { _Pos = pos; }
	XMFLOAT3 Right() const { return _Right; }
	void Right(const XMFLOAT3& right) { _Right = right; }
	XMFLOAT3 Up() const { return _Up; }
	void Up(const XMFLOAT3& up) { _Up = up; }
	XMFLOAT3 LookTo() const { return _LookTo; }
	void LookTo(const XMFLOAT3& look) { _LookTo = look; }

	void ForBack(float units) {
		_Pos.x += _LookTo.x * units;
		_Pos.z += _LookTo.z * units;
		if (this->m_Free) {
			_Pos.y += _LookTo.y * units;
		}
	}

	void LeftRight(float units) {
		_Pos.x += _Right.x * units;
		_Pos.z += _Right.z * units;
		if (this->m_Free) {
			_Pos.y += _Right.y * units;
		}
	}

	void UpDown(float units) {
		_Pos.x += _Up.x * units;
		_Pos.z += _Up.z * units;
		if (this->m_Free) {
			_Pos.y += _Up.y * units;
		}
	}

	void Pitch(float angle) {
		if (angle > XM_PI - 0.1f)
			angle = XM_PI - 0.1f;
		else if (angle < 0.1f)
			angle = 0.1f;

		XMMATRIX rot = XMMatrixRotationAxis(XMLoadFloat3(&_Right), angle);
		XMStoreFloat3(&_Up, XMVector3TransformCoord(XMLoadFloat3(&_Up), rot));
		XMStoreFloat3(&_LookTo, XMVector3TransformCoord(XMLoadFloat3(&_LookTo), rot));
	}

	void Yaw(float angle) {
		if (angle > XM_2PI)
			angle -= XM_2PI;

		XMMATRIX rot;
		if (this->m_Free) {
			rot = XMMatrixRotationAxis(XMLoadFloat3(&_Up), angle);
		}
		else {
			rot = XMMatrixRotationY(angle);
		}
		XMStoreFloat3(&_Right, XMVector3TransformCoord(XMLoadFloat3(&_Right), rot));
		XMStoreFloat3(&_LookTo, XMVector3TransformCoord(XMLoadFloat3(&_LookTo), rot));
	}

	void Roll(float angle) {
		if (this->m_Free) {
			XMMATRIX rot = XMMatrixRotationAxis(XMLoadFloat3(&_LookTo), angle);
			XMStoreFloat3(&_Up, XMVector3TransformCoord(XMLoadFloat3(&_Up), rot));
			XMStoreFloat3(&_Right, XMVector3TransformCoord(XMLoadFloat3(&_Right), rot));
		}
	}

	XMMATRIX View() {
		XMVECTOR look = XMVector3Normalize(XMLoadFloat3(&_LookTo));
		XMVECTOR right = XMLoadFloat3(&_Right);
		XMVECTOR up = XMVector3Normalize(XMVector3Cross(look, right));
		right = XMVector3Normalize(XMVector3Cross(up, look));
		XMStoreFloat3(&_LookTo, look);
		XMStoreFloat3(&_Right, right);
		XMStoreFloat3(&_Up, up);

		return XMMatrixLookToLH(XMLoadFloat3(&this->_Pos), look, up);
	}

	HRESULT Initialize(HINSTANCE hInst, int width, int height) {
		HRESULT hr = DirectInput8Create(hInst,
			DIRECTINPUT_VERSION,
			IID_IDirectInput8W,
			(void**)this->m_Input.ReleaseAndGetAddressOf(), NULL);

		hr = this->m_Input->CreateDevice(GUID_SysKeyboard, this->m_Keyboard.ReleaseAndGetAddressOf(), NULL);
		hr = this->m_Keyboard->SetDataFormat(&c_dfDIKeyboard);
		hr = this->m_Input->CreateDevice(GUID_SysMouse, this->m_Mouse.ReleaseAndGetAddressOf(), NULL);
		hr = this->m_Mouse->SetDataFormat(&c_dfDIMouse2);

		//this->m_Projection = XMMatrixOrthographicLH(width, height, 0.1f, 1000.0f);
		this->m_Projection = XMMatrixPerspectiveLH(width, height, 0.1f, 1000.0f);

		return hr;
	}

	void Update() {
		DIMOUSESTATE2 stateMouse;
		this->m_Mouse->Acquire();
		this->m_Mouse->GetDeviceState(sizeof(stateMouse), &stateMouse);
		if (stateMouse.lX != this->m_MouseLastState.lX) {
			this->Yaw( stateMouse.lX * PitchYawRatio);
		}
		if (stateMouse.lY != this->m_MouseLastState.lY) {
			this->Pitch( stateMouse.lY * PitchYawRatio);
		}
		if (stateMouse.lZ != this->m_MouseLastState.lZ) {
			this->ForBack(stateMouse.lZ * WalkSpeed) ;
		}
		m_MouseLastState = stateMouse;

		BYTE stateKeyboard[256];
		this->m_Keyboard->Acquire();
		this->m_Keyboard->GetDeviceState(256, &stateKeyboard);

		//this->m_View = XMMatrixRotationRollPitchYaw(this->m_pitch, this->m_yaw, 0.0f);
		//this->m_View *= XMMatrixTranslationFromVector(this->_Pos);
	}
};

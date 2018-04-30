#include "pch.h"
#include "DxCamera.h"


DxCamera::DxCamera()
{
}


DxCamera::~DxCamera()
{
}

void DxCamera::Render()
{
	float yaw, pitch, roll;
	pitch = m_Rotation.x * 0.0174532925f;
	yaw = m_Rotation.y * 0.0174532925f;
	roll = m_Rotation.z * 0.0174532925f;
	XMMATRIX matRotation = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

	// Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.
	XMVECTOR vLookAt{0.0f, 0.0f, 1.0f, 0.0f};
	vLookAt = XMVector3TransformCoord(vLookAt, matRotation);
	XMVECTOR vUp{ 0.0F, 1.0F, 0.0F, 0.0F };
	vUp = XMVector3TransformCoord(vUp, matRotation);

	// Translate the rotated camera position to the location of the viewer.
	vLookAt = XMVectorAdd(m_Position, vLookAt);

	// Finally create the view matrix from the three updated vectors.
	m_matView = XMMatrixLookAtLH(m_Position, vLookAt, vUp);
}

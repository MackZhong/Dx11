#include "pch.h"
#include "Dx11Input.h"

Dx11Input::Dx11Input()
{
}

Dx11Input::~Dx11Input()
{
}

bool Dx11Input::Initialize(HWND m_hWnd)
{
	for (int k = 0; k < 256; k++) {
		m_bKeyDown[k] = false;
	}
	return true;
}

void Dx11Input::Destroy()
{
}

bool Dx11Input::IsKeyDown(unsigned int keyCode)
{
	return m_bKeyDown[keyCode];
}

void Dx11Input::KeyDown(unsigned int keyCode)
{
	m_bKeyDown[keyCode] = true;
}

void Dx11Input::KeyUp(unsigned int keyCode)
{
	m_bKeyDown[keyCode] = false;
}
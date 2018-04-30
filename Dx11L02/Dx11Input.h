#pragma once
class Dx11Input
{
private:
	bool m_bKeyDown[256];

public:
	Dx11Input();
	~Dx11Input();

public:
	bool Initialize(HWND m_hWnd);
	void Destroy();
	bool IsKeyDown(unsigned int keyCode);
	void KeyDown(unsigned int keyCode);
	void KeyUp(unsigned int keyCode);
};

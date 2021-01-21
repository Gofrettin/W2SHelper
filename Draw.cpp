#include "framework.h"

#define M_PI 3.141f

int Draw::Red =		D3DCOLOR_ARGB(255, 255, 000, 000);
int Draw::White =	D3DCOLOR_ARGB(255, 255, 255, 255);
int Draw::Orange =	D3DCOLOR_ARGB(255, 255, 125, 000);
int Draw::Yellow =	D3DCOLOR_ARGB(255, 255, 255, 000);
int Draw::Miku =	D3DCOLOR_ARGB(255, 000, 255, 255);
int Draw::Green =	D3DCOLOR_ARGB(255, 000, 255, 000);

IDirect3D9Ex*			d3dObject;
IDirect3DDevice9Ex*		d3dDevice;
D3DPRESENT_PARAMETERS	d3dparams;
ID3DXFont*				d3dFont;
ID3DXFont*				d3despFont;
ID3DXFont*				d3dFontWarning;
ID3DXLine*				d3dLine;

void Draw::Initial(HWND overlayHWND) {
	if (FAILED(Direct3DCreate9Ex(D3D_SDK_VERSION, &d3dObject))) {
		exit(1);
	}
	ZeroMemory(&d3dparams, sizeof(d3dparams));

	d3dparams.Windowed = true;
	d3dparams.BackBufferWidth = Game::width;
	d3dparams.BackBufferHeight = Game::height;
	d3dparams.BackBufferFormat = D3DFMT_A8R8G8B8;

	d3dparams.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dparams.hDeviceWindow = overlayHWND;
	d3dparams.MultiSampleQuality = D3DMULTISAMPLE_NONE;
	d3dparams.EnableAutoDepthStencil = true;
	d3dparams.AutoDepthStencilFormat = D3DFMT_D16;

	HRESULT res = d3dObject->CreateDeviceEx(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, overlayHWND, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dparams, 0, &d3dDevice);
	if (FAILED(res))
		exit(1);

	//D3DXCreateFont(d3dDevice, 50, 0, FW_BOLD, 1, false, DEFAULT_CHARSET, OUT_DEVICE_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH, L"Comic Sans", &d3dFont);
	D3DXCreateFont(d3dDevice, 20, 0, FW_BOLD, 1, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Arial", &d3dFont);
	D3DXCreateFont(d3dDevice, 13, 0, 0, 0, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, L"Arial", &d3despFont);

	// VERY IMPORTANT , IF MISSING, NULL PTR EXCEPTION WILL HAPPEN WHILE DRAW LINES AND RECTANGLES   !!!
	if (!d3dLine)
		D3DXCreateLine(d3dDevice, &d3dLine);
}

void Draw::Render()
{
	if (d3dDevice == nullptr)
		return;
	d3dDevice->Clear(0, 0, D3DCLEAR_TARGET, 0, 1.0f, 0);
	d3dDevice->BeginScene();
	if (Game::winHWND == GetForegroundWindow())
	{
		// Gets self info
		Self::GetBasicInfo();
		Self::GetViewMatrix();
		Self::GetHealth();
		Self::GetWorldCoords();
		// Draws self info
		
	}
	d3dDevice->EndScene();
	d3dDevice->PresentEx(0, 0, 0, 0, 0);

	return;
}

void Draw::String(int x, int y, const char* string, D3DCOLOR color)
{
	RECT Position;
	Position.left = x + 1;
	Position.top = y + 1;
	//d3dFont->DrawTextA(0, string, strlen(string), &Position, DT_NOCLIP, color);
	d3dFont->DrawTextA(0, string, strlen(string), &Position, DT_NOCLIP, color);

	//Position.left = x;
	//Position.top = y;
	//d3dFont->DrawTextA(0, string, strlen(string), &Position, DT_NOCLIP, color);

}

// [x, y} is the middle point if each entity
void Draw::Rect(int x, int y, int width, int height, D3DCOLOR color)
{
	int startX = x - width / 2, startY = y - height / 2;
	D3DXVECTOR2 Rect[5];
	Rect[0] = D3DXVECTOR2(startX, startY);
	Rect[1] = D3DXVECTOR2(startX + width, startY);
	Rect[2] = D3DXVECTOR2(startX + width, startY + height);
	Rect[3] = D3DXVECTOR2(startX, startY + height);
	Rect[4] = D3DXVECTOR2(startX, startY);
	d3dLine->SetWidth(2);
	d3dLine->Draw(Rect, 5, color);
}

// ?
void Draw::BorderedRect(int x, int y, int width, int height, int fa, int fr, int fg, int fb, D3DCOLOR color)
{
	int startX = x - width / 2, startY = y - height / 2;
	D3DXVECTOR2 Fill[2];
	Fill[0] = D3DXVECTOR2(startX + width / 2, startY);
	Fill[1] = D3DXVECTOR2(startX + width / 2, startY + height);
	d3dLine->SetWidth(width);
	d3dLine->Draw(Fill, 2, D3DCOLOR_ARGB(fa, fr, fg, fb));

	D3DXVECTOR2 Rect[5];
	Rect[0] = D3DXVECTOR2(startX, startY);
	Rect[1] = D3DXVECTOR2(startX + width, startY);
	Rect[2] = D3DXVECTOR2(startX + width, startY + height);
	Rect[3] = D3DXVECTOR2(startX, startY + height);
	Rect[4] = D3DXVECTOR2(startX, startY);
	d3dLine->SetWidth(1);
	d3dLine->Draw(Rect, 5, color);
}

void Draw::Line(int x, int y, int x1, int y2, D3DCOLOR color)
{
	D3DXVECTOR2 Line[2];
	Line[0] = D3DXVECTOR2(x, y);
	Line[1] = D3DXVECTOR2(x1, y2);
	d3dLine->SetWidth(2);
	d3dLine->Draw(Line, 2, color);
}

void Draw::HealthBar(int x, int y, int width, int height, float health, float maxHealth, D3DCOLOR color)
{
	int startX = x - width / 2, startY = y + height / 2;
	D3DXVECTOR2 Rect[2];
	Rect[0] = D3DXVECTOR2(startX, startY);
	Rect[1] = D3DXVECTOR2(startX, startY - (health / maxHealth) * height);
	d3dLine->SetWidth(3);
	d3dLine->Draw(Rect, 2, color);
}

void Draw::Circle(int x, int y, int radius, D3DCOLOR color)
{
	D3DXVECTOR2 DotPoints[128];
	D3DXVECTOR2 Line[128];
	int Points = 0;
	for (float i = 0; i < M_PI * 2.0f; i += 0.1f)
	{
		float PointOriginX = x + radius * cos(i);
		float PointOriginY = y + radius * sin(i);
		float PointOriginX2 = radius * cos(i + 0.1) + x;
		float PointOriginY2 = radius * sin(i + 0.1) + y;
		DotPoints[Points] = D3DXVECTOR2(PointOriginX, PointOriginY);
		DotPoints[Points + 1] = D3DXVECTOR2(PointOriginX2, PointOriginY2);
		Points += 2;
	}
	d3dLine->Draw(DotPoints, Points, color);
}

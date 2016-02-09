#pragma once
#include <Utils/Windows.h>
#include <d3dx9.h>

enum FONTALIGNMENT { FA_LEFT, FA_CENTER, FA_RIGHT, FA_TOPRIGHT, FA_TOPLEFT, FA_BOTTOMRIGHT, FA_BOTTOMLEFT };

#define SAFE_RELEASE(x) if(x) { x->Release(); x = NULL; } 

class C2DFont
{
public:
	C2DFont();
	~C2DFont();

	BOOL Initialize(LPDIRECT3DDEVICE9 pDevice, const char* faceName, int size, BOOL bold = FALSE,

		BOOL italic = FALSE);

	void Print(const char* text, int xPosition, int yPosition, DWORD color, LPD3DXSPRITE sprite = NULL,

		int textBoxWidth = 0, int textBoxHeight = 0, FONTALIGNMENT alignment = FA_LEFT) const;

	void OnLostDevice() const;

	void OnResetDevice() const;

	void Release();

private:

	LPD3DXFONT m_pFont;

};


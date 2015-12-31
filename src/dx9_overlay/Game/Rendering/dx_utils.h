#pragma once
#include <d3dx9.h>

#define DRAW_FVF (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)

class CD3DFont;

namespace Drawing
{
	struct stVertex
	{
		float fX, fY, fZ, fRHW;
		D3DCOLOR dwColor;
	};

	void DrawBox(float x, float y, float w, float h, DWORD color, LPDIRECT3DDEVICE9 pDevice);
	void DrawRectangular(float X, float Y, float Width, float Height, float Thickness, D3DCOLOR Color, LPDIRECT3DDEVICE9 pDev);
	void DrawPrimtive(LPDIRECT3DDEVICE9 pDevice, D3DPRIMITIVETYPE PrimitiveType, UINT PrimitiveCount, CONST void* pVertexStreamZeroData, UINT VertexStreamZeroStride, DWORD FVF = DRAW_FVF);

	void DrawSprite(LPD3DXSPRITE SpriteInterface, LPDIRECT3DTEXTURE9 TextureInterface, int PosX, int PosY, float ScaleX, float ScaleY, int Rotation, int Align);
}

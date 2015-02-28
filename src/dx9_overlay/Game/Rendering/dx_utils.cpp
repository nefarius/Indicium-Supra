#include "dx_utils.h"
#include "D3DFont.h"

void Drawing::DrawBox(float x, float y, float w, float h, DWORD color, LPDIRECT3DDEVICE9 pDevice)
{
	stVertex q[4];

	q[0].dwColor = q[1].dwColor = q[2].dwColor = q[3].dwColor = color;

	q[0].fZ = q[1].fZ = q[2].fZ = q[3].fZ = 0;
	q[0].fRHW = q[1].fRHW = q[2].fRHW = q[3].fRHW = 0;

	q[0].fX = q[2].fX = x;
	q[0].fY = q[1].fY = y;
	q[1].fX = q[3].fX = x + w;
	q[2].fY = q[3].fY = y + h;

	Drawing::DrawPrimtive(pDevice, D3DPT_TRIANGLESTRIP, 2, q, sizeof(stVertex));
}

void Drawing::DrawRectangular(float X, float Y, float Width, float Height, float Thickness, D3DCOLOR Color, LPDIRECT3DDEVICE9 pDev)
{
	Drawing::DrawBox(X, Y + Height - Thickness, Width, Thickness, Color, pDev);
	Drawing::DrawBox(X, Y, Thickness, Height, Color, pDev);
	Drawing::DrawBox(X, Y, Width, Thickness, Color, pDev);
	Drawing::DrawBox(X + Width - Thickness, Y, Thickness, Height, Color, pDev);
}

void Drawing::DrawPrimtive(LPDIRECT3DDEVICE9 pDevice, D3DPRIMITIVETYPE PrimitiveType, UINT PrimitiveCount, CONST void* pVertexStreamZeroData, UINT VertexStreamZeroStride, DWORD FVF)
{
	DWORD dwOldFVF;
	LPDIRECT3DPIXELSHADER9 ppixelShader;
	LPDIRECT3DBASETEXTURE9 ppTexture;

	pDevice->GetFVF(&dwOldFVF);
	pDevice->GetPixelShader(&ppixelShader);
	pDevice->GetTexture(0, &ppTexture);

	pDevice->SetPixelShader(NULL);
	pDevice->SetTexture(0, NULL);
	pDevice->SetFVF(FVF);

	pDevice->DrawPrimitiveUP(PrimitiveType, PrimitiveCount, pVertexStreamZeroData, VertexStreamZeroStride);

	pDevice->SetPixelShader(ppixelShader);
	pDevice->SetTexture(0, ppTexture);
	pDevice->SetFVF(dwOldFVF);
}

void Drawing::DrawSprite(LPD3DXSPRITE SpriteInterface, LPDIRECT3DTEXTURE9 TextureInterface, int PosX, int PosY, int Rotation, int Align)
{
	if (SpriteInterface == NULL || TextureInterface == NULL)
		return;

	D3DXVECTOR3 Vec;

	Vec.x = (FLOAT) PosX;
	Vec.y = (FLOAT) PosY;
	Vec.z = (FLOAT)0.0f;

	D3DXMATRIX mat;
	D3DXVECTOR2 scaling(1.0f, 1.0f);
	D3DSURFACE_DESC desc;

	TextureInterface->GetLevelDesc(0, &desc);

	D3DXVECTOR2 spriteCentre;
	if (Align == 1)
		spriteCentre = D3DXVECTOR2((FLOAT) desc.Width / 2, (FLOAT) desc.Height / 2);
	else
		spriteCentre = D3DXVECTOR2(0, 0);

	D3DXVECTOR2 trans = D3DXVECTOR2(0, 0);
	D3DXMatrixTransformation2D(&mat, NULL, 0.0, &scaling, &spriteCentre, (FLOAT) Rotation, &trans);

	SpriteInterface->SetTransform(&mat);
	SpriteInterface->Begin(D3DXSPRITE_ALPHABLEND);
	SpriteInterface->Draw(TextureInterface, NULL, NULL, &Vec, 0xFFFFFFFF);
	SpriteInterface->End();
}

void Drawing::InitFont(LPDIRECT3DDEVICE9 pDevice, const char *str, int size, DWORD dwFlags, class CD3DFont **pFont)
{
	if(pFont)
	{
		if(*pFont)
		{
			delete *pFont;
			*pFont = NULL;
		}
	}

	(*pFont) = new CD3DFont(str, size, dwFlags);
	(*pFont)->InitDeviceObjects(pDevice);
	(*pFont)->RestoreDeviceObjects();
}

void Drawing::ResetFont(class CD3DFont **pFont)
{
	if (pFont){
		if (*pFont)
		{
			delete *pFont;
			*pFont = NULL;
			pFont = NULL;
		}
	}
}

void Drawing::DrawFont(CD3DFont **pFont, int x, int y, DWORD dwColor, const char* strText, DWORD dwFlags, DWORD dwBackgroundColor)
{
	if (pFont)
		if (*pFont)
			(*pFont)->DrawText((float)x, (float)y, dwColor, strText, dwFlags);
	
}
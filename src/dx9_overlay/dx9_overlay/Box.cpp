#include "Box.h"

void CBox::DrawBox(int x,int y, int w, int h, D3DCOLOR dwColor, LPDIRECT3DDEVICE9 pDev)
{
	struct stVertex
	{
		float x,y,z,t;
		D3DCOLOR Color;
	}	q[4];

	q[0].Color = q[1].Color = q[2].Color = q[3].Color = dwColor;

	q[0].z  = q[1].z = q[2].z  = q[3].z  = 0;
	q[0].t  = q[1].t = q[2].t  = q[3].t  = 0;

	q[0].x = q[2].x = (float)x;
	q[0].y = q[1].y = (float)y;
	q[1].x = q[3].x = (float)(x + w);
	q[2].y = q[3].y = (float)(y + h);

	pDev->SetTexture( 0,NULL);
	pDev->SetFVF(D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1);
	pDev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, q, sizeof(stVertex));
}

void CBox::DrawRectangular(int X, int Y, int Width, int Height, int Thickness, D3DCOLOR Color, LPDIRECT3DDEVICE9 pDev)
{
	DrawBox(X, Y + Height - Thickness, Width, Thickness, Color,pDev);
	DrawBox( X, Y, Thickness, Height, Color,pDev);
	DrawBox(X, Y, Width, Thickness, Color,pDev);
	DrawBox(X + Width - Thickness, Y, Thickness, Height, Color,pDev);
}

CBox::CBox(CRenderer *renderer,  int x, int y, int w, int h, D3DCOLOR color, bool show) : CRenderBase(renderer), m_bShown(false)
{
	SetPos(x, y);
	SetBoxWidth(w);
	SetBoxHeight(h);
	SetBoxColor(color);
	SetShown(show);

	SetBoxColor(color);
	SetBorderWidth(0);
	SetBorderShown(false);
}


void CBox::SetPos(int x,int y)
{
	m_iX = x, m_iY = y;
}

void CBox::SetBorderColor(D3DCOLOR dwColor)
{
	m_dwBorderColor = dwColor;
}

void CBox::SetBoxColor(D3DCOLOR dwColor)
{
	m_dwBoxColor = dwColor;
}

void CBox::SetBorderWidth(DWORD dwWidth)
{
	m_dwBorderWidth = dwWidth;
}

void CBox::SetBoxWidth(DWORD dwWidth)
{
	m_dwBoxWidth = dwWidth;
}

void CBox::SetBoxHeight(DWORD dwHeight)
{
	m_dwBoxHeight = dwHeight;
}

void CBox::SetBorderShown(bool b)
{
	m_bBorderShown = b;
}

void CBox::SetShown(bool b)
{
	m_bShown = b;
}

void CBox::Draw(IDirect3DDevice9 *pDevice)
{
	if(!m_bShown)
		return;

	D3DVIEWPORT9 view;
	RECT box; 

	pDevice->GetViewport(&view);

	float fFactor[] = 
	{	
		(float)m_iX/(float)800, 
		(float)m_iY/(float)600, 
		(float)m_dwBoxWidth/(float)800, 
		(float)m_dwBoxHeight/(float)600
	};

	int iX = (int)(view.Width * fFactor[0]);
	int iY = (int)(view.Height * fFactor[1]);
	int iBoxWidth = (int)(view.Width * fFactor[2]);
	int iBoxHeight = (int)(view.Height * fFactor[3]);

	DrawBox(iX, iY, iBoxWidth, iBoxHeight, m_dwBoxColor, pDevice);


	if(m_bBorderShown)
		DrawRectangular(iX,iY,iBoxWidth,iBoxHeight,m_dwBorderWidth,m_dwBorderColor,pDevice);
}

void CBox::Reset(IDirect3DDevice9 *pDevice)
{
	
}

void CBox::Show()
{
	SetShown(true);
}

void CBox::Hide()
{
	SetShown(false);
}

void CBox::ReleaseResourcesForDeletion(IDirect3DDevice9 *pDevice)
{
	m_bShown = false;
	m_bBorderShown = false;
}

bool CBox::CanBeDeleted()
{
	return true;
}

bool CBox::LoadResource(IDirect3DDevice9 *pDevice)
{
	return true;
}

void CBox::FirstDrawAfterReset(IDirect3DDevice9 *pDevice)
{
	
}
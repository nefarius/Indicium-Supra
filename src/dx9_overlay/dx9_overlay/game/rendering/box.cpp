#include "box.h"
#include "dx_utils.h"

box::box(renderer *renderer,  int x, int y, int w, int h, D3DCOLOR color, bool show) : renderbase(renderer), m_bShown(false)
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


void box::SetPos(int x,int y)
{
	m_iX = x, m_iY = y;
}

void box::SetBorderColor(D3DCOLOR dwColor)
{
	m_dwBorderColor = dwColor;
}

void box::SetBoxColor(D3DCOLOR dwColor)
{
	m_dwBoxColor = dwColor;
}

void box::SetBorderWidth(DWORD dwWidth)
{
	m_dwBorderWidth = dwWidth;
}

void box::SetBoxWidth(DWORD dwWidth)
{
	m_dwBoxWidth = dwWidth;
}

void box::SetBoxHeight(DWORD dwHeight)
{
	m_dwBoxHeight = dwHeight;
}

void box::SetBorderShown(bool b)
{
	m_bBorderShown = b;
}

void box::SetShown(bool b)
{
	m_bShown = b;
}

void box::Draw(IDirect3DDevice9 *pDevice)
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

	Drawing::DrawBox(iX, iY, iBoxWidth, iBoxHeight, m_dwBoxColor, pDevice);


	if(m_bBorderShown)
		Drawing::DrawRectangular(iX, iY, iBoxWidth, iBoxHeight, m_dwBorderWidth, m_dwBorderColor, pDevice);
}

void box::Reset(IDirect3DDevice9 *pDevice)
{
	
}

void box::Show()
{
	SetShown(true);
}

void box::Hide()
{
	SetShown(false);
}

void box::ReleaseResourcesForDeletion(IDirect3DDevice9 *pDevice)
{
	m_bShown = false;
	m_bBorderShown = false;
}

bool box::CanBeDeleted()
{
	return true;
}

bool box::LoadResource(IDirect3DDevice9 *pDevice)
{
	return true;
}

void box::FirstDrawAfterReset(IDirect3DDevice9 *pDevice)
{
	
}
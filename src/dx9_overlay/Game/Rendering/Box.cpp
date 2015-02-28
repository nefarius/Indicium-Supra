#include "Box.h"
#include "dx_utils.h"

CBox::CBox(CRenderer *renderer,  int x, int y, int w, int h, D3DCOLOR color, bool show) : CRenderBase(renderer), m_bShown(false)
{
	setPos(x, y);
	setBoxWidth(w);
	setBoxHeight(h);
	setBoxColor(color);
	setShown(show);

	setBoxColor(color);
	setBorderWidth(0);
	setBorderShown(false);
}


void CBox::setPos(int x,int y)
{
	m_iX = x, m_iY = y;
}

void CBox::setBorderColor(D3DCOLOR dwColor)
{
	m_dwBorderColor = dwColor;
}

void CBox::setBoxColor(D3DCOLOR dwColor)
{
	m_dwBoxColor = dwColor;
}

void CBox::setBorderWidth(DWORD dwWidth)
{
	m_dwBorderWidth = dwWidth;
}

void CBox::setBoxWidth(DWORD dwWidth)
{
	m_dwBoxWidth = dwWidth;
}

void CBox::setBoxHeight(DWORD dwHeight)
{
	m_dwBoxHeight = dwHeight;
}

void CBox::setBorderShown(bool b)
{
	m_bBorderShown = b;
}

void CBox::setShown(bool b)
{
	m_bShown = b;
}

void CBox::draw(IDirect3DDevice9 *pDevice)
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

void CBox::reset(IDirect3DDevice9 *pDevice)
{
	
}

void CBox::show()
{
	setShown(true);
}

void CBox::hide()
{
	setShown(false);
}

void CBox::releaseResourcesForDeletion(IDirect3DDevice9 *pDevice)
{
	m_bShown = false;
	m_bBorderShown = false;
}

bool CBox::canBeDeleted()
{
	return true;
}

bool CBox::loadResource(IDirect3DDevice9 *pDevice)
{
	return true;
}

void CBox::firstDrawAfterReset(IDirect3DDevice9 *pDevice)
{
	
}
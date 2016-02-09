#include "Box.h"
#include "dx_utils.h"

Box::Box(Renderer *renderer,  int x, int y, int w, int h, D3DCOLOR color, bool show)
	: RenderBase(renderer), m_bShown(false)
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


void Box::setPos(int x,int y)
{
	m_iX = x, m_iY = y;
}

void Box::setBorderColor(D3DCOLOR dwColor)
{
	m_dwBorderColor = dwColor;
}

void Box::setBoxColor(D3DCOLOR dwColor)
{
	m_dwBoxColor = dwColor;
}

void Box::setBorderWidth(DWORD dwWidth)
{
	m_dwBorderWidth = dwWidth;
}

void Box::setBoxWidth(DWORD dwWidth)
{
	m_dwBoxWidth = dwWidth;
}

void Box::setBoxHeight(DWORD dwHeight)
{
	m_dwBoxHeight = dwHeight;
}

void Box::setBorderShown(bool b)
{
	m_bBorderShown = b;
}

void Box::setShown(bool b)
{
	m_bShown = b;
}

void Box::draw(IDirect3DDevice9 *pDevice)
{
	if(!m_bShown)
		return;

	float x = (float)calculatedXPos(m_iX);
	float y = (float)calculatedYPos(m_iY);
	float w = (float)calculatedXPos(m_dwBoxWidth);
	float h = (float)calculatedYPos(m_dwBoxHeight);

	Drawing::DrawBox(x, y, w, h, m_dwBoxColor, pDevice);

	if(m_bBorderShown)
		Drawing::DrawRectangular(x, y, w, h, (float)m_dwBorderWidth, m_dwBorderColor, pDevice);
}

void Box::reset(IDirect3DDevice9 *pDevice)
{
	
}

void Box::show()
{
	setShown(true);
}

void Box::hide()
{
	setShown(false);
}

void Box::releaseResourcesForDeletion(IDirect3DDevice9 *pDevice)
{
	m_bShown = false;
	m_bBorderShown = false;
}

bool Box::canBeDeleted()
{
	return true;
}

bool Box::loadResource(IDirect3DDevice9 *pDevice)
{
	return true;
}

void Box::firstDrawAfterReset(IDirect3DDevice9 *pDevice)
{
	
}
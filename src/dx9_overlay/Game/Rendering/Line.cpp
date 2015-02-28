#include "Line.h"

CLine::CLine(CRenderer *renderer, int x1,int y1,int x2,int y2,int width,D3DCOLOR color, bool bShow)
	: CRenderBase(renderer), m_Line(NULL)
{
	setPos(x1,y1,x2,y2);
	setWidth(width);
	setColor(color);
	setShown(bShow);
}

void CLine::setPos(int x1,int y1,int x2,int y2)
{
	m_X1 = x1, m_X2 = x2;
	m_Y1 = y1, m_Y2 = y2;
}

void CLine::setWidth(int width)
{
	m_Width = width;
}

void CLine::setColor(D3DCOLOR color)
{
	m_Color = color;
}

void CLine::setShown(bool show)
{
	m_bShow = show;
}

void CLine::draw(IDirect3DDevice9 *pDevice)
{
	if(!m_bShow || m_Line == NULL)
		return;

	D3DVIEWPORT9 view;
	pDevice->GetViewport(&view);

	float fFactor[4] = 
	{ 
		(float)m_X1/(float)800, (float)m_X2/(float)800,
		(float)m_Y1/(float)600, (float)m_Y2/(float)600				
	};

	D3DXVECTOR2	LinePos[2];

	m_Line->SetAntialias(TRUE);
	m_Line->SetWidth((FLOAT)m_Width);

	m_Line->Begin();

	LinePos[0].x = (float)view.Width  * fFactor[0];
	LinePos[0].y = (float)view.Height * fFactor[2];
	LinePos[1].x = (float)view.Width  * fFactor[1];
	LinePos[1].y = (float)view.Height * fFactor[3];

	m_Line->Draw(LinePos,2,m_Color);
	m_Line->End();	
}

void CLine::reset(IDirect3DDevice9 *pDevice)
{
	if(m_Line)
	{
		m_Line->OnLostDevice();
		m_Line->OnResetDevice();
	}
}

void CLine::show()
{
	setShown(true);
}

void CLine::hide()
{
	setShown(false);
}

void CLine::releaseResourcesForDeletion(IDirect3DDevice9 *pDevice)
{
	if(m_Line)
	{
		m_Line->Release();
		m_Line = NULL;
	}
}

bool CLine::canBeDeleted()
{
	return (m_Line == NULL) ? true : false;
}

bool CLine::loadResource(IDirect3DDevice9 *pDevice)
{
	if(m_Line)
	{
		m_Line->Release();
		m_Line = NULL;
	}

	D3DXCreateLine(pDevice,&m_Line);

	return m_Line != NULL;
}

void CLine::firstDrawAfterReset(IDirect3DDevice9 *pDevice)
{

}
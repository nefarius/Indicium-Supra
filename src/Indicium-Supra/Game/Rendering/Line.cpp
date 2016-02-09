#include "Line.h"

Line::Line(Renderer *renderer, int x1,int y1,int x2,int y2,int width,D3DCOLOR color, bool bShow)
	: RenderBase(renderer), m_Line(NULL)
{
	setPos(x1,y1,x2,y2);
	setWidth(width);
	setColor(color);
	setShown(bShow);
}

void Line::setPos(int x1,int y1,int x2,int y2)
{
	m_X1 = x1, m_X2 = x2;
	m_Y1 = y1, m_Y2 = y2;
}

void Line::setWidth(int width)
{
	m_Width = width;
}

void Line::setColor(D3DCOLOR color)
{
	m_Color = color;
}

void Line::setShown(bool show)
{
	m_bShow = show;
}

void Line::draw(IDirect3DDevice9 *pDevice)
{
	if(!m_bShow || m_Line == NULL)
		return;

	D3DXVECTOR2	LinePos[2];

	m_Line->SetAntialias(TRUE);
	m_Line->SetWidth((FLOAT)m_Width);

	m_Line->Begin();

	LinePos[0].x = (float)calculatedXPos(m_X1);
	LinePos[0].y = (float)calculatedYPos(m_Y1);
	LinePos[1].x = (float)calculatedXPos(m_X2);
	LinePos[1].y = (float)calculatedYPos(m_Y2);

	m_Line->Draw(LinePos,2,m_Color);
	m_Line->End();	
}

void Line::reset(IDirect3DDevice9 *pDevice)
{
	if(m_Line)
	{
		m_Line->OnLostDevice();
		m_Line->OnResetDevice();
	}
}

void Line::show()
{
	setShown(true);
}

void Line::hide()
{
	setShown(false);
}

void Line::releaseResourcesForDeletion(IDirect3DDevice9 *pDevice)
{
	if(m_Line)
	{
		m_Line->Release();
		m_Line = NULL;
	}
}

bool Line::canBeDeleted()
{
	return (m_Line == NULL) ? true : false;
}

bool Line::loadResource(IDirect3DDevice9 *pDevice)
{
	if(m_Line)
	{
		m_Line->Release();
		m_Line = NULL;
	}

	D3DXCreateLine(pDevice,&m_Line);

	return m_Line != NULL;
}

void Line::firstDrawAfterReset(IDirect3DDevice9 *pDevice)
{

}
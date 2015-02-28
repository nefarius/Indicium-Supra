#pragma once
#include <d3dx9.h>

#include "RenderBase.h"

class Line : public RenderBase
{
public:
	Line(Renderer *renderer, int x1,int y1,int x2,int y2,int width,D3DCOLOR color, bool bShow);

	void setPos(int x1,int y1,int x2,int y2);
	void setWidth(int width);
	void setColor(D3DCOLOR color);
	void setShown(bool show);

protected:
	virtual void draw(IDirect3DDevice9 *pDevice) sealed;
	virtual void reset(IDirect3DDevice9 *pDevice) sealed;

	virtual void show() sealed;
	virtual void hide() sealed;

	virtual void releaseResourcesForDeletion(IDirect3DDevice9 *pDevice) sealed;
	virtual bool canBeDeleted() sealed;

	virtual bool loadResource(IDirect3DDevice9 *pDevice) override sealed;
	virtual void firstDrawAfterReset(IDirect3DDevice9 *pDevice) override sealed;

private:
	int	m_X1, m_X2, m_Y1, m_Y2, m_Width;

	bool m_bShow;

	D3DCOLOR m_Color;

	LPD3DXLINE m_Line;
};
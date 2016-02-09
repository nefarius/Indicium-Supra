#pragma once
#include <d3dx9.h>

#include "RenderBase.h"

class Box : public RenderBase
{
public:
	Box(Renderer *renderer, int x, int y, int w, int h, D3DCOLOR color, bool show);

	void setPos(int x,int y);
	void setBorderColor(D3DCOLOR dwColor);
	void setBoxColor(D3DCOLOR dwColor);
	void setBorderWidth(DWORD dwWidth);
	void setBoxWidth(DWORD dwWidth);
	void setBoxHeight(DWORD dwHeight);
	void setBorderShown(bool b);
	void setShown(bool b);

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
	bool m_bShown, m_bBorderShown;
	D3DCOLOR m_dwBoxColor, m_dwBorderColor;
	DWORD m_dwBorderWidth, m_dwBoxWidth, m_dwBoxHeight;
	int	m_iX, m_iY;
};
#pragma once
#include <d3dx9.h>

#include "RenderBase.h"

class CBox : public CRenderBase
{
public:
	CBox(CRenderer *renderer, int x, int y, int w, int h, D3DCOLOR color, bool show);

	static void		DrawBox(int x,int y, int w, int h, D3DCOLOR dwColor, LPDIRECT3DDEVICE9 pDev);
	static void		DrawRectangular(int x,int y, int width,int height,int thickness, D3DCOLOR dwColor, LPDIRECT3DDEVICE9 pDev);

	void			SetPos(int x,int y);
	void			SetBorderColor(D3DCOLOR dwColor);
	void			SetBoxColor(D3DCOLOR dwColor);
	void			SetBorderWidth(DWORD dwWidth);
	void			SetBoxWidth(DWORD dwWidth);
	void			SetBoxHeight(DWORD dwHeight);
	void			SetBorderShown(bool b);
	void			SetShown(bool b);

protected:
	virtual void Draw(IDirect3DDevice9 *pDevice) sealed;
	virtual void Reset(IDirect3DDevice9 *pDevice) sealed;

	virtual void Show() sealed;
	virtual void Hide() sealed;

	virtual void ReleaseResourcesForDeletion(IDirect3DDevice9 *pDevice) sealed;
	virtual bool CanBeDeleted() sealed;

	virtual bool LoadResource(IDirect3DDevice9 *pDevice) override sealed;
	virtual void FirstDrawAfterReset(IDirect3DDevice9 *pDevice) override sealed;

private:
	bool				m_bShown,
						m_bBorderShown;

	D3DCOLOR			m_dwBoxColor,
						m_dwBorderColor;

	DWORD				m_dwBorderWidth,
						m_dwBoxWidth,
						m_dwBoxHeight;

	int					m_iX,
						m_iY;
};
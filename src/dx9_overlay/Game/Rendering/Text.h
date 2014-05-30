#pragma once
#include <string>
#include <d3dx9.h>

#include "D3DFont.h"
#include "RenderBase.h"

class CText : public CRenderBase
{
public:
	CText(CRenderer *renderer, const char *font,int iFontSize,bool Bold,bool Italic,int x,int y,D3DCOLOR color,const char *text, bool bShadow, bool bShow);

	bool UpdateText(const char *Font,int FontSize,bool Bold,bool Italic);
	void SetText(const char *str);
	void SetColor(D3DCOLOR color);
	void SetPos(int x,int y);
	void SetShown(bool bShow);
	void SetShadow(bool bShadow);

protected:
	virtual void Draw(IDirect3DDevice9 *pDevice) sealed;
	virtual void Reset(IDirect3DDevice9 *pDevice) sealed;

	virtual void Show() override sealed;
	virtual void Hide() override sealed;

	virtual void ReleaseResourcesForDeletion(IDirect3DDevice9 *pDevice) override sealed;
	virtual bool CanBeDeleted() override sealed;

	virtual bool LoadResource(IDirect3DDevice9 *pDevice) override sealed;
	virtual void FirstDrawAfterReset(IDirect3DDevice9 *pDevice) override sealed;

private:
	std::string			m_Text,
						m_Font;

	int					m_X,
						m_Y,
						m_FontSize;

	D3DCOLOR			m_Color;

	CD3DFont*			m_D3DFont;

	bool				m_bShown,
						m_bShadow,
						m_bItalic,
						m_bBold;					
};


#include "Text.h"
#include "dx_utils.h"

CText::CText(CRenderer *renderer, const char *font,int iFontSize,bool Bold,bool Italic,int x,int y,D3DCOLOR color,const char *text, bool bShadow, bool bShow)
	: CRenderBase(renderer), m_D3DFont(NULL)
{
	setPos(x,y);
	setColor(color);
	setText(text);
	setShadow(bShadow);
	setShown(bShow);

	m_Font = font;
	m_FontSize = iFontSize;
	m_bBold = Bold;
	m_bItalic = Italic;
}


bool CText::updateText(const char *Font,int FontSize,bool Bold,bool Italic)
{
	m_Font = Font;
	m_FontSize = FontSize;
	m_bBold = Bold;
	m_bItalic = Italic;

	changeResource();

	return true;
}

void CText::setText(const char *str)
{
	m_Text = str;
}

void CText::setColor(D3DCOLOR color)
{
	m_Color = color;
}

void CText::setPos(int x,int y)
{
	m_X = x, m_Y = y;
}

void CText::setShown(bool bShown)
{
	m_bShown = bShown;
}

void CText::setShadow(bool bShadow)
{
	m_bShadow = bShadow;
}

void CText::draw(IDirect3DDevice9 *pDevice)
{
	if(!m_bShown)
		return;

	D3DVIEWPORT9 view;
	pDevice->GetViewport(&view);

	float fFactor[3] = { (float)m_X/(float)800, (float)m_Y/(float)600, (float)m_FontSize / (float)600};

	if(m_bShadow)
	{
		const float shadowOffset = 1.0f;

		Drawing::DrawFont(&m_D3DFont, view.Width * fFactor[0] - shadowOffset, view.Height * fFactor[1], D3DCOLOR_ARGB(255, 0, 0, 0), m_Text.c_str());
		Drawing::DrawFont(&m_D3DFont, view.Width * fFactor[0] + shadowOffset, view.Height * fFactor[1], D3DCOLOR_ARGB(255, 0, 0, 0), m_Text.c_str());
		Drawing::DrawFont(&m_D3DFont, view.Width * fFactor[0], view.Height * fFactor[1] - shadowOffset, D3DCOLOR_ARGB(255, 0, 0, 0), m_Text.c_str());
		Drawing::DrawFont(&m_D3DFont, view.Width * fFactor[0], view.Height * fFactor[1] + shadowOffset, D3DCOLOR_ARGB(255, 0, 0, 0), m_Text.c_str());
	}

	Drawing::DrawFont(&m_D3DFont, view.Width * fFactor[0], view.Height * fFactor[1], m_Color, m_Text.c_str(), D3DFONT_COLORTABLE);
}

void CText::reset(IDirect3DDevice9 *pDevice)
{
	Drawing::ResetFont(&m_D3DFont);
}

void CText::show()
{
	setShown(true);
}

void CText::hide()
{
	setShown(false);
}

void CText::releaseResourcesForDeletion(IDirect3DDevice9 *pDevice)
{
	Drawing::ResetFont(&m_D3DFont);
}

bool CText::canBeDeleted()
{
	return (m_D3DFont == NULL) ? true : false;
}

bool CText::loadResource(IDirect3DDevice9 *pDevice)
{
	D3DVIEWPORT9 view;
	pDevice->GetViewport(&view);

	float fFactor[3] = { (float)m_X/(float)800, (float)m_Y/(float)600, (float)m_FontSize / (float)600};

	Drawing::InitFont(pDevice, m_Font.c_str(), int(view.Height * fFactor[2]), (m_bBold)?D3DFONT_BOLD:0 | (m_bItalic)?D3DFONT_ITALIC:0 | D3DFONT_FILTERED, &m_D3DFont);

	return m_D3DFont != NULL;
}

void CText::firstDrawAfterReset(IDirect3DDevice9 *pDevice)
{
	loadResource(pDevice);
}
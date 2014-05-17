#include "text.h"
#include "dx_utils.h"

text::text(renderer *renderer, const char *font,int iFontSize,bool Bold,bool Italic,int x,int y,D3DCOLOR color,const char *text, bool bShadow, bool bShow)
	: renderbase(renderer), m_D3DFont(NULL)
{
	SetPos(x,y);
	SetColor(color);
	SetText(text);
	SetShadow(bShadow);
	SetShown(bShow);

	m_Font = font;
	m_FontSize = iFontSize;
	m_bBold = Bold;
	m_bItalic = Italic;
}


bool text::UpdateText(const char *Font,int FontSize,bool Bold,bool Italic)
{
	m_Font = Font;
	m_FontSize = FontSize;
	m_bBold = Bold;
	m_bItalic = Italic;

	ChangeResource();

	return true;
}

void text::SetText(const char *str)
{
	m_Text = str;
}

void text::SetColor(D3DCOLOR color)
{
	m_Color = color;
}

void text::SetPos(int x,int y)
{
	m_X = x, m_Y = y;
}

void text::SetShown(bool bShown)
{
	m_bShown = bShown;
}

void text::SetShadow(bool bShadow)
{
	m_bShadow = bShadow;
}

void text::Draw(IDirect3DDevice9 *pDevice)
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

void text::Reset(IDirect3DDevice9 *pDevice)
{
	Drawing::ResetFont(&m_D3DFont);
}

void text::Show()
{
	SetShown(true);
}

void text::Hide()
{
	SetShown(false);
}

void text::ReleaseResourcesForDeletion(IDirect3DDevice9 *pDevice)
{
	Drawing::ResetFont(&m_D3DFont);
}

bool text::CanBeDeleted()
{
	return (m_D3DFont == NULL) ? true : false;
}

bool text::LoadResource(IDirect3DDevice9 *pDevice)
{
	D3DVIEWPORT9 view;
	pDevice->GetViewport(&view);

	float fFactor[3] = { (float)m_X/(float)800, (float)m_Y/(float)600, (float)m_FontSize / (float)600};

	Drawing::InitFont(pDevice, m_Font.c_str(), int(view.Height * fFactor[2]), (m_bBold)?D3DFONT_BOLD:0 | (m_bItalic)?D3DFONT_ITALIC:0 | D3DFONT_FILTERED, &m_D3DFont);

	return m_D3DFont != NULL;
}

void text::FirstDrawAfterReset(IDirect3DDevice9 *pDevice)
{
	LoadResource(pDevice);
}
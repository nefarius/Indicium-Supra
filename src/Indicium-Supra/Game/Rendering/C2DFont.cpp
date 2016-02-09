#include "C2DFont.h"
#include <boost/log/trivial.hpp>


C2DFont::C2DFont()
{
	m_pFont = nullptr;
}


C2DFont::~C2DFont()
{
	Release();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Initialize the font
Parameters:
[in] pDevice – D3D Device for D3DXCreateFont call
[in] faceName – Font name
[in] size – Font size
[in] bold – Bold if TRUE
[in] italic – Italicized if TRUE
Returns: TRUE if font was created successfully, FALSE otherwise.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
BOOL C2DFont::Initialize(LPDIRECT3DDEVICE9 pDevice, const char* faceName, int size, BOOL bold, BOOL italic)
{
	SAFE_RELEASE(m_pFont);
	HRESULT hr = 0;
	hr = D3DXCreateFont(pDevice, -size, 0, bold ? 800 : 0, 1, italic, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, faceName, &m_pFont);
	if (FAILED(hr))
	{
		BOOST_LOG_TRIVIAL(fatal) << "D3DXCreateFont() failed.";
		return FALSE;
	}
	return TRUE;
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Print some 2D text.
Parameters:
[in] text – Text to print
[in] xPosition – X position in window coordinates
[in] yPosition – Y position in window coordinates
[in] color – Color of the text.
[in] sprite – Sprite for batch printing
[in] textBoxWidth – Width to constrain text in
[in] textBoxHeight – Height to constrain text in
[in] format – FONTALIGNMENT value.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void C2DFont::Print(const char* text, int xPosition, int yPosition, DWORD color, LPD3DXSPRITE sprite, 
int textBoxWidth, int textBoxHeight, FONTALIGNMENT alignment) const
{
	if (!m_pFont)
	{
		return;
	}
	DWORD format = DT_EXPANDTABS;
	if (textBoxWidth == 0)
	{
		format |= DT_NOCLIP;
	}
	else
	{
		format |= DT_WORDBREAK;
		switch (alignment)
		{
		case FA_LEFT:
			format |= DT_LEFT;
			break;
		case FA_CENTER:
			format |= DT_CENTER;
			break;
		case FA_RIGHT:
			format |= DT_RIGHT;
			break;
		case FA_TOPRIGHT:
			format |= DT_RIGHT | DT_TOP;
			break;
		case FA_BOTTOMRIGHT:
			format |= DT_RIGHT | DT_BOTTOM;
			break;
		case FA_TOPLEFT:
			format |= DT_LEFT | DT_TOP;
			break;
		case FA_BOTTOMLEFT:
			format |= DT_LEFT | DT_BOTTOM;
			break;
		}
		if (textBoxHeight == 0)
		{
			// A width is specified, but not a height. 
			// Make it seem like height is infinite 
			textBoxHeight = 2000;
		}
	}
	RECT rect = { xPosition, yPosition, xPosition + textBoxWidth, yPosition + textBoxHeight };
	m_pFont->DrawText(sprite, text, -1, &rect, format, color);
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Releases video resources. Call whenever the device is lost or before reseting the device.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void C2DFont::OnLostDevice() const
{
	if (m_pFont)
	{
		m_pFont->OnLostDevice();
	}
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Re-acquire video resources. Call after device is reset
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void C2DFont::OnResetDevice() const
{
	if (m_pFont)
	{
		m_pFont->OnResetDevice();
	}
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Release interfaces
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void C2DFont::Release() 
{
	SAFE_RELEASE(m_pFont);
}
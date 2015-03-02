#include <Utils/SafeBlock.h>

#include "Text.h"
#include "dx_utils.h"

Text::Text(Renderer *renderer, const std::string& font,int iFontSize,bool Bold,bool Italic,int x,int y,D3DCOLOR color,const std::string& text, bool bShadow, bool bShow)
	: RenderBase(renderer), m_D3DFont(NULL)
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


bool Text::updateText(const std::string& Font,int FontSize,bool Bold,bool Italic)
{
	m_Font = Font;
	m_FontSize = FontSize;
	m_bBold = Bold;
	m_bItalic = Italic;

	changeResource();
	return true;
}

void Text::setText(const std::string& str)
{
	m_Text = str;
}

void Text::setColor(D3DCOLOR color)
{
	m_Color = color;
}

void Text::setPos(int x,int y)
{
	m_X = x, m_Y = y;
}

void Text::setShown(bool bShown)
{
	m_bShown = bShown;
}

void Text::setShadow(bool bShadow)
{
	m_bShadow = bShadow;
}

void Text::draw(IDirect3DDevice9 *pDevice)
{
	if(!m_bShown)
		return;

	int x = calculatedXPos(m_X);
	int y = calculatedYPos(m_Y);

	if(m_bShadow)
	{
		const int shadowOffset = 1;

		drawText(x - shadowOffset, y, D3DCOLOR_ARGB(255, 0, 0, 0), m_Text);
		drawText(x + shadowOffset, y, D3DCOLOR_ARGB(255, 0, 0, 0), m_Text);
		drawText(x, y - shadowOffset, D3DCOLOR_ARGB(255, 0, 0, 0), m_Text);
		drawText(x, y + shadowOffset, D3DCOLOR_ARGB(255, 0, 0, 0), m_Text);
	}

	drawText(x, y, m_Color, m_Text, D3DFONT_COLORTABLE);
}

void Text::reset(IDirect3DDevice9 *pDevice)
{
	resetFont();
}

void Text::show()
{
	setShown(true);
}

void Text::hide()
{
	setShown(false);
}

void Text::releaseResourcesForDeletion(IDirect3DDevice9 *pDevice)
{
	resetFont();
}

bool Text::canBeDeleted()
{
	return m_D3DFont == nullptr;
}

bool Text::loadResource(IDirect3DDevice9 *pDevice)
{
	initFont(pDevice);
	return true;
}

void Text::firstDrawAfterReset(IDirect3DDevice9 *pDevice)
{
	loadResource(pDevice);
}

void Text::initFont(IDirect3DDevice9 *pDevice)
{
	int size = calculatedYPos(m_FontSize);

	m_D3DFont = std::make_shared<CD3DFont>(m_Font.c_str(), size, (m_bBold) ? D3DFONT_BOLD : 0 | (m_bItalic) ? D3DFONT_ITALIC : 0 | D3DFONT_FILTERED);
	m_D3DFont->InitDeviceObjects(pDevice);
	m_D3DFont->RestoreDeviceObjects();
}

void Text::resetFont()
{
	m_D3DFont.reset();
}

bool Text::drawText(int x, int y, DWORD dwColor, const std::string& strText, DWORD dwFlags /*= 0L*/)
{
	return safeExecuteWithValidation([&](){
		m_D3DFont->DrawTextA((float)x, (float)y, dwColor, m_Text.c_str(), dwFlags);
	});
}

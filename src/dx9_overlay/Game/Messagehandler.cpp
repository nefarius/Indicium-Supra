#include "Messagehandler.h"
#include "Game.h"
#include "Rendering/Text.h"
#include "Rendering/Box.h"
#include "Rendering/Line.h"
#include "Rendering/Image.h"
#include "Rendering/Renderer.h"
#include "Rendering/RenderBase.h"

#define READ(X, Y) BITSTREAM_READ(bsIn, X, Y);
#define WRITE(X) bsOut << X;

void TextCreate(CBitStream& bsIn, CBitStream& bsOut)
{
	READ(std::string, Font); READ(int, FontSize); 
	READ(bool, bBold); READ(bool, bItalic);
	READ(int, x); READ(int, y);
	READ(unsigned int, color);
	READ(std::string, Text);
	READ(bool, bShadow);
	READ(bool, bShow);

	boost::shared_ptr<CText> obj(new CText(&g_pRenderer, Font.c_str(), FontSize, bBold, bItalic, x, y, color, Text.c_str(), bShadow, bShow));

	WRITE(g_pRenderer.Add(obj));
}

void TextDestroy(CBitStream& bsIn, CBitStream& bsOut)
{
	READ(int, id);
	WRITE(int(g_pRenderer.Remove(id)));
}

void TextSetShadow(CBitStream& bsIn, CBitStream& bsOut)
{
	READ(int, id); READ(bool, bShadow);

	auto obj = g_pRenderer.Get<CText>(id);
	if (obj)
	{
		obj->SetShadow(bShadow);
		WRITE(int(1));
	}
	else
		WRITE(int(0));
}

void TextSetShown(CBitStream& bsIn, CBitStream& bsOut)
{
	READ(int, id); READ(bool, bShown);

	auto obj = g_pRenderer.Get<CText>(id);
	if (obj)
	{
		obj->SetShown(bShown);
		WRITE(int(1));
	}
	else
		WRITE(int(0));
}

void TextSetColor(CBitStream& bsIn, CBitStream& bsOut)
{
	READ(int, id); READ(unsigned int, color);

	auto obj = g_pRenderer.Get<CText>(id);
	if (obj)
	{
		obj->SetColor(color);
		WRITE(int(1));
	}
	else
		WRITE(int(0));
}

void TextSetPos(CBitStream& bsIn, CBitStream& bsOut)
{
	READ(int, id); READ(int, x); READ(int, y);

	auto obj = g_pRenderer.Get<CText>(id);
	if (obj)
	{
		obj->SetPos(x, y);
		WRITE(int(1));
	}
	else
		WRITE(int(0));
}

void TextSetString(CBitStream& bsIn, CBitStream& bsOut)
{
	READ(int, id); READ(std::string, str);

	auto obj = g_pRenderer.Get<CText>(id);
	if (obj)
	{
		obj->SetText(str.c_str());
		WRITE(int(1));
	}
	else
		WRITE(int(0));
}

void TextUpdate(CBitStream& bsIn, CBitStream& bsOut)
{
	READ(int, id); READ(std::string, Font); READ(int, FontSize); 
	READ(bool, bBold); READ(bool, bItalic);

	auto obj = g_pRenderer.Get<CText>(id);
	if (obj)
	{
		WRITE(int(obj->UpdateText(Font.c_str(), FontSize, bBold, bItalic)));
	}
	else
		WRITE((int) 0);
}

void BoxCreate(CBitStream& bsIn, CBitStream& bsOut)
{
	READ(int, x); READ(int, y); READ(int, w); READ(int, h); READ(unsigned int, dwColor); READ(bool, bShow);

	boost::shared_ptr<CBox> obj(new CBox(&g_pRenderer, x, y, w, h, dwColor, bShow));

	WRITE(g_pRenderer.Add(obj));
}

void BoxDestroy(CBitStream& bsIn, CBitStream& bsOut)
{
	READ(int, id);
	WRITE((int) g_pRenderer.Remove(id));
}

void BoxSetShown(CBitStream& bsIn, CBitStream& bsOut)
{
	READ(int, id); READ(bool, bShown);

	auto obj = g_pRenderer.Get<CBox>(id);
	if (obj)
	{
		obj->SetShown(bShown);
		WRITE(int(1));
	}
	else
		WRITE(int(0));
}

void BoxSetBorder(CBitStream& bsIn, CBitStream& bsOut)
{
	READ(int, id); READ(int, height); READ(bool, bShown);

	auto obj = g_pRenderer.Get<CBox>(id);
	if (obj)
	{
		obj->SetBorderWidth(height);
		obj->SetBorderShown(bShown);
		WRITE(int(1));
	}
	else
		WRITE(int(0));
}

void BoxSetBorderColor(CBitStream& bsIn, CBitStream& bsOut)
{
	READ(int, id); READ(unsigned int, dwColor);

	auto obj = g_pRenderer.Get<CBox>(id);
	if (obj)
	{
		obj->SetBorderColor(dwColor);
		WRITE(int(1));
	}
	else
		WRITE(int(0));
}

void BoxSetColor(CBitStream& bsIn, CBitStream& bsOut)
{
	READ(int, id); READ(unsigned int, dwColor);

	auto obj = g_pRenderer.Get<CBox>(id);
	if (obj)
	{
		obj->SetBoxColor(dwColor);
		WRITE(int(1));
	}
	else
		WRITE(int(0));
}

void BoxSetHeight(CBitStream& bsIn, CBitStream& bsOut)
{
	READ(int, id); READ(int, height);

	auto obj = g_pRenderer.Get<CBox>(id);
	if (obj)
	{
		obj->SetBoxHeight(height);
		WRITE(int(1));
	}
	else
		WRITE(int(0));
}

void BoxSetPos(CBitStream& bsIn, CBitStream& bsOut)
{
	READ(int, id); READ(int, x); READ(int, y);

	auto obj = g_pRenderer.Get<CBox>(id);
	if (obj)
	{
		obj->SetPos(x, y);
		WRITE(int(1));
	}
	else
		WRITE(int(0));
}

void BoxSetWidth(CBitStream& bsIn, CBitStream& bsOut)
{
	READ(int, id); READ(int, width);

	auto obj = g_pRenderer.Get<CBox>(id);
	if (obj)
	{
		obj->SetBoxWidth(width);
		WRITE(int(1));
	}
	else
		WRITE(int(0));
}

void LineCreate(CBitStream& bsIn, CBitStream& bsOut)
{
	READ(int, x1); READ(int, y1); READ(int, x2); READ(int, y2); READ(int, width); READ(unsigned int, color);
	READ(bool, bShow);

	boost::shared_ptr<CLine> obj(new CLine(&g_pRenderer, x1, y1, x2, y2, width, color, bShow));

	WRITE(g_pRenderer.Add(obj));
}

void LineDestroy(CBitStream& bsIn, CBitStream& bsOut)
{
	READ(int, id);
	WRITE((int) g_pRenderer.Remove(id));
}

void LineSetShown(CBitStream& bsIn, CBitStream& bsOut)
{
	READ(int, id); READ(bool, bShown);

	auto obj = g_pRenderer.Get<CLine>(id);
	if (obj)
	{
		obj->SetShown(bShown);
		WRITE(int(1));
	}
	else
		WRITE(int(0));
}

void LineSetColor(CBitStream& bsIn, CBitStream& bsOut)
{
	READ(int, id); READ(unsigned int, color);

	auto obj = g_pRenderer.Get<CLine>(id);
	if (obj)
	{
		obj->SetColor(color);
		WRITE(int(1));
	}
	else
		WRITE(int(0));
}

void LineSetWidth(CBitStream& bsIn, CBitStream& bsOut)
{
	READ(int, id); READ(int, width);

	auto obj = g_pRenderer.Get<CLine>(id);
	if (obj)
	{
		obj->SetWidth(width);
		WRITE(int(1));
	}
	else
		WRITE(int(0));
}

void LineSetPos(CBitStream& bsIn, CBitStream& bsOut)
{
	READ(int, id); READ(int, x1); READ(int, y1); READ(int, x2); READ(int, y2);

	auto obj = g_pRenderer.Get<CLine>(id);
	if (obj)
	{
		obj->SetPos(x1, y1, x2, y2);
		WRITE(int(1));
	}
	else
		WRITE(int(0));
}

void ImageCreate(CBitStream& bsIn, CBitStream& bsOut)
{
	READ(std::string, path); READ(int, x); READ(int, y); READ(int, rotation); READ(int, align); READ(bool, show);
	
	boost::shared_ptr<CImage> obj(new CImage(&g_pRenderer, path.c_str(), x, y, rotation, align, show));

	WRITE(g_pRenderer.Add(obj));
}

void ImageDestroy(CBitStream& bsIn, CBitStream& bsOut)
{
	READ(int, id);
	WRITE((int) g_pRenderer.Remove(id));
}

void ImageSetShown(CBitStream& bsIn, CBitStream& bsOut)
{
	READ(int, id); READ(bool, bShow);

	auto obj = g_pRenderer.Get<CImage>(id);
	if (obj)
	{
		obj->SetShown(bShow);
		WRITE(int(1));
	}
	else
		WRITE(int(0));
}

void ImageSetAlign(CBitStream& bsIn, CBitStream& bsOut)
{
	READ(int, id); READ(int, align);

	auto obj = g_pRenderer.Get<CImage>(id);
	if (obj)
	{
		obj->SetAlign(align);
		WRITE(int(1));
	}
	else
		WRITE(int(0));
}

void ImageSetPos(CBitStream& bsIn, CBitStream& bsOut)
{
	READ(int, id); READ(int, x); READ(int, y);

	auto obj = g_pRenderer.Get<CImage>(id);
	if (obj)
	{
		obj->SetPos(x, y);
		WRITE(int(1));
	}
	else
		WRITE(int(0));
}

void ImageSetRotation(CBitStream& bsIn, CBitStream& bsOut)
{
	READ(int, id); READ(int, rotation);

	auto obj = g_pRenderer.Get<CImage>(id);
	if (obj)
	{
		obj->SetRotation(rotation);
		WRITE(int(1));
	}
	else
		WRITE(int(0));
}


void DestroyAllVisual(CBitStream& bsIn, CBitStream& bsOut)
{
	g_pRenderer.DestroyAll();
}

void ShowAllVisual(CBitStream& bsIn, CBitStream& bsOut)
{
	g_pRenderer.ShowAll();
}

void HideAllVisual(CBitStream& bsIn, CBitStream& bsOut)
{
	g_pRenderer.HideAll();
}

void GetFrameRate(CBitStream& bsIn, CBitStream& bsOut)
{
	WRITE(g_pRenderer.GetFrameRate());
}

void GetScreenSpecs(CBitStream& bsIn, CBitStream& bsOut)
{
	WRITE(g_pRenderer.GetScreenWidth()); WRITE(g_pRenderer.GetScreenHeight());
}
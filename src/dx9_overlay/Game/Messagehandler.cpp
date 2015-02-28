#include "Messagehandler.h"
#include "Game.h"
#include "Rendering/Text.h"
#include "Rendering/Box.h"
#include "Rendering/Line.h"
#include "Rendering/Image.h"
#include "Rendering/Renderer.h"
#include "Rendering/RenderBase.h"

#define READ(X, Y) SERIALIZATION_READ(bsIn, X, Y);
#define WRITE(X) bsOut << X;

void TextCreate(CSerializer& bsIn, CSerializer& bsOut)
{
	READ(std::string, Font); 
	READ(int, FontSize); 
	READ(bool, bBold);
	READ(bool, bItalic);
	READ(int, x); 
	READ(int, y);
	READ(unsigned int, color);
	READ(std::string, Text);
	READ(bool, bShadow);
	READ(bool, bShow);

	auto obj = std::make_shared<CText>(&g_pRenderer, Font.c_str(), FontSize, bBold, bItalic, x, y, color, Text.c_str(), bShadow, bShow);

	WRITE(g_pRenderer.add(obj));
}

void TextDestroy(CSerializer& bsIn, CSerializer& bsOut)
{
	READ(int, id);
	WRITE(int(g_pRenderer.remove(id)));
}

void TextSetShadow(CSerializer& bsIn, CSerializer& bsOut)
{
	READ(int, id); 
	READ(bool, bShadow);

	auto obj = g_pRenderer.get<CText>(id);
	if (obj)
	{
		obj->setShadow(bShadow);
		WRITE(int(1));
	}
	else
		WRITE(int(0));
}

void TextSetShown(CSerializer& bsIn, CSerializer& bsOut)
{
	READ(int, id); 
	READ(bool, bShown);

	auto obj = g_pRenderer.get<CText>(id);
	if (obj)
	{
		obj->setShown(bShown);
		WRITE(int(1));
	}
	else
		WRITE(int(0));
}

void TextSetColor(CSerializer& bsIn, CSerializer& bsOut)
{
	READ(int, id); 
	READ(unsigned int, color);

	auto obj = g_pRenderer.get<CText>(id);
	if (obj)
	{
		obj->setColor(color);
		WRITE(int(1));
	}
	else
		WRITE(int(0));
}

void TextSetPos(CSerializer& bsIn, CSerializer& bsOut)
{
	READ(int, id); 
	READ(int, x); 
	READ(int, y);

	auto obj = g_pRenderer.get<CText>(id);
	if (obj)
	{
		obj->setPos(x, y);
		WRITE(int(1));
	}
	else
		WRITE(int(0));
}

void TextSetString(CSerializer& bsIn, CSerializer& bsOut)
{
	READ(int, id); 
	READ(std::string, str);

	auto obj = g_pRenderer.get<CText>(id);
	if (obj)
	{
		obj->setText(str.c_str());
		WRITE(int(1));
	}
	else
		WRITE(int(0));
}

void TextUpdate(CSerializer& bsIn, CSerializer& bsOut)
{
	READ(int, id); 
	READ(std::string, Font); 
	READ(int, FontSize); 
	READ(bool, bBold); 
	READ(bool, bItalic);

	auto obj = g_pRenderer.get<CText>(id);
	if (obj)
	{
		WRITE(int(obj->updateText(Font.c_str(), FontSize, bBold, bItalic)));
	}
	else
		WRITE((int) 0);
}

void BoxCreate(CSerializer& bsIn, CSerializer& bsOut)
{
	READ(int, x); 
	READ(int, y); 
	READ(int, w); 
	READ(int, h); 
	READ(unsigned int, dwColor); 
	READ(bool, bShow);

	WRITE(g_pRenderer.add(std::make_shared<CBox>(&g_pRenderer, x, y, w, h, dwColor, bShow)));
}

void BoxDestroy(CSerializer& bsIn, CSerializer& bsOut)
{
	READ(int, id);
	WRITE((int) g_pRenderer.remove(id));
}

void BoxSetShown(CSerializer& bsIn, CSerializer& bsOut)
{
	READ(int, id); 
	READ(bool, bShown);

	auto obj = g_pRenderer.get<CBox>(id);
	if (obj)
	{
		obj->setShown(bShown);
		WRITE(int(1));
	}
	else
		WRITE(int(0));
}

void BoxSetBorder(CSerializer& bsIn, CSerializer& bsOut)
{
	READ(int, id); 
	READ(int, height); 
	READ(bool, bShown);

	auto obj = g_pRenderer.get<CBox>(id);
	if (obj)
	{
		obj->setBorderWidth(height);
		obj->setBorderShown(bShown);
		WRITE(int(1));
	}
	else
		WRITE(int(0));
}

void BoxSetBorderColor(CSerializer& bsIn, CSerializer& bsOut)
{
	READ(int, id); 
	READ(unsigned int, dwColor);

	auto obj = g_pRenderer.get<CBox>(id);
	if (obj)
	{
		obj->setBorderColor(dwColor);
		WRITE(int(1));
	}
	else
		WRITE(int(0));
}

void BoxSetColor(CSerializer& bsIn, CSerializer& bsOut)
{
	READ(int, id); 
	READ(unsigned int, dwColor);

	auto obj = g_pRenderer.get<CBox>(id);
	if (obj)
	{
		obj->setBoxColor(dwColor);
		WRITE(int(1));
	}
	else
		WRITE(int(0));
}

void BoxSetHeight(CSerializer& bsIn, CSerializer& bsOut)
{
	READ(int, id); 
	READ(int, height);

	auto obj = g_pRenderer.get<CBox>(id);
	if (obj)
	{
		obj->setBoxHeight(height);
		WRITE(int(1));
	}
	else
		WRITE(int(0));
}

void BoxSetPos(CSerializer& bsIn, CSerializer& bsOut)
{
	READ(int, id); 
	READ(int, x); 
	READ(int, y);

	auto obj = g_pRenderer.get<CBox>(id);
	if (obj)
	{
		obj->setPos(x, y);
		WRITE(int(1));
	}
	else
		WRITE(int(0));
}

void BoxSetWidth(CSerializer& bsIn, CSerializer& bsOut)
{
	READ(int, id); 
	READ(int, width);

	auto obj = g_pRenderer.get<CBox>(id);
	if (obj)
	{
		obj->setBoxWidth(width);
		WRITE(int(1));
	}
	else
		WRITE(int(0));
}

void LineCreate(CSerializer& bsIn, CSerializer& bsOut)
{
	READ(int, x1); 
	READ(int, y1); 
	READ(int, x2); 
	READ(int, y2); 
	READ(int, width); 
	READ(unsigned int, color);
	READ(bool, bShow);

	WRITE(g_pRenderer.add(std::make_shared<CLine>(&g_pRenderer, x1, y1, x2, y2, width, color, bShow)));
}

void LineDestroy(CSerializer& bsIn, CSerializer& bsOut)
{
	READ(int, id);
	WRITE((int) g_pRenderer.remove(id));
}

void LineSetShown(CSerializer& bsIn, CSerializer& bsOut)
{
	READ(int, id); 
	READ(bool, bShown);

	auto obj = g_pRenderer.get<CLine>(id);
	if (obj)
	{
		obj->setShown(bShown);
		WRITE(int(1));
	}
	else
		WRITE(int(0));
}

void LineSetColor(CSerializer& bsIn, CSerializer& bsOut)
{
	READ(int, id); 
	READ(unsigned int, color);

	auto obj = g_pRenderer.get<CLine>(id);
	if (obj)
	{
		obj->setColor(color);
		WRITE(int(1));
	}
	else
		WRITE(int(0));
}

void LineSetWidth(CSerializer& bsIn, CSerializer& bsOut)
{
	READ(int, id); 
	READ(int, width);

	auto obj = g_pRenderer.get<CLine>(id);
	if (obj)
	{
		obj->setWidth(width);
		WRITE(int(1));
	}
	else
		WRITE(int(0));
}

void LineSetPos(CSerializer& bsIn, CSerializer& bsOut)
{
	READ(int, id); 
	READ(int, x1); 
	READ(int, y1); 
	READ(int, x2); 
	READ(int, y2);

	auto obj = g_pRenderer.get<CLine>(id);
	if (obj)
	{
		obj->setPos(x1, y1, x2, y2);
		WRITE(int(1));
	}
	else
		WRITE(int(0));
}

void ImageCreate(CSerializer& bsIn, CSerializer& bsOut)
{
	READ(std::string, path); 
	READ(int, x); 
	READ(int, y); 
	READ(int, rotation); 
	READ(int, align); 
	READ(bool, show);
	
	WRITE(g_pRenderer.add(std::make_shared<CImage>(&g_pRenderer, path.c_str(), x, y, rotation, align, show)));
}

void ImageDestroy(CSerializer& bsIn, CSerializer& bsOut)
{
	READ(int, id);
	WRITE((int) g_pRenderer.remove(id));
}

void ImageSetShown(CSerializer& bsIn, CSerializer& bsOut)
{
	READ(int, id); 
	READ(bool, bShow);

	auto obj = g_pRenderer.get<CImage>(id);
	if (obj)
	{
		obj->setShown(bShow);
		WRITE(int(1));
	}
	else
		WRITE(int(0));
}

void ImageSetAlign(CSerializer& bsIn, CSerializer& bsOut)
{
	READ(int, id); 
	READ(int, align);

	auto obj = g_pRenderer.get<CImage>(id);
	if (obj)
	{
		obj->setAlign(align);
		WRITE(int(1));
	}
	else
		WRITE(int(0));
}

void ImageSetPos(CSerializer& bsIn, CSerializer& bsOut)
{
	READ(int, id); 
	READ(int, x); 
	READ(int, y);

	auto obj = g_pRenderer.get<CImage>(id);
	if (obj)
	{
		obj->setPos(x, y);
		WRITE(int(1));
	}
	else
		WRITE(int(0));
}

void ImageSetRotation(CSerializer& bsIn, CSerializer& bsOut)
{
	READ(int, id); 
	READ(int, rotation);

	auto obj = g_pRenderer.get<CImage>(id);
	if (obj)
	{
		obj->setRotation(rotation);
		WRITE(int(1));
	}
	else
		WRITE(int(0));
}


void DestroyAllVisual(CSerializer& bsIn, CSerializer& bsOut)
{
	g_pRenderer.destroyAll();
}

void ShowAllVisual(CSerializer& bsIn, CSerializer& bsOut)
{
	g_pRenderer.showAll();
}

void HideAllVisual(CSerializer& bsIn, CSerializer& bsOut)
{
	g_pRenderer.hideAll();
}

void GetFrameRate(CSerializer& bsIn, CSerializer& bsOut)
{
	WRITE(g_pRenderer.frameRate());
}

void GetScreenSpecs(CSerializer& bsIn, CSerializer& bsOut)
{
	WRITE(g_pRenderer.screenWidth()); 
	WRITE(g_pRenderer.screenHeight());
}
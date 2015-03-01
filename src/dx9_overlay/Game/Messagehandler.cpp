#include "Messagehandler.h"
#include "Game.h"
#include "Rendering/Text.h"
#include "Rendering/Box.h"
#include "Rendering/Line.h"
#include "Rendering/Image.h"
#include "Rendering/Renderer.h"
#include "Rendering/RenderBase.h"

#define READ(X, Y) SERIALIZATION_READ(serializerIn, X, Y);
#define WRITE(X) serializerOut << X;

void TextCreate(Serializer& serializerIn, Serializer& serializerOut)
{
	READ(std::string, Font); 
	READ(int, FontSize); 
	READ(bool, bBold);
	READ(bool, bItalic);
	READ(int, x); 
	READ(int, y);
	READ(unsigned int, color);
	READ(std::string, string);
	READ(bool, bShadow);
	READ(bool, bShow);

	WRITE(g_pRenderer.add(std::make_shared<Text>(&g_pRenderer, Font.c_str(), FontSize, bBold, bItalic, x, y, color, string.c_str(), bShadow, bShow)));
}

void TextDestroy(Serializer& serializerIn, Serializer& serializerOut)
{
	READ(int, id);
	WRITE(int(g_pRenderer.remove(id)));
}

void TextSetShadow(Serializer& serializerIn, Serializer& serializerOut)
{
	READ(int, id); 
	READ(bool, bShadow);

	auto obj = g_pRenderer.getAs<Text>(id);
	if (obj)
	{
		obj->setShadow(bShadow);
		WRITE(int(1));
	}
	else
		WRITE(int(0));
}

void TextSetShown(Serializer& serializerIn, Serializer& serializerOut)
{
	READ(int, id); 
	READ(bool, bShown);

	auto obj = g_pRenderer.getAs<Text>(id);
	if (obj)
	{
		obj->setShown(bShown);
		WRITE(int(1));
	}
	else
		WRITE(int(0));
}

void TextSetColor(Serializer& serializerIn, Serializer& serializerOut)
{
	READ(int, id); 
	READ(unsigned int, color);

	auto obj = g_pRenderer.getAs<Text>(id);
	if (obj)
	{
		obj->setColor(color);
		WRITE(int(1));
	}
	else
		WRITE(int(0));
}

void TextSetPos(Serializer& serializerIn, Serializer& serializerOut)
{
	READ(int, id); 
	READ(int, x); 
	READ(int, y);

	auto obj = g_pRenderer.getAs<Text>(id);
	if (obj)
	{
		obj->setPos(x, y);
		WRITE(int(1));
	}
	else
		WRITE(int(0));
}

void TextSetString(Serializer& serializerIn, Serializer& serializerOut)
{
	READ(int, id); 
	READ(std::string, str);

	auto obj = g_pRenderer.getAs<Text>(id);
	if (obj)
	{
		obj->setText(str.c_str());
		WRITE(int(1));
	}
	else
		WRITE(int(0));
}

void TextUpdate(Serializer& serializerIn, Serializer& serializerOut)
{
	READ(int, id); 
	READ(std::string, Font); 
	READ(int, FontSize); 
	READ(bool, bBold); 
	READ(bool, bItalic);

	auto obj = g_pRenderer.getAs<Text>(id);
	if (obj)
	{
		WRITE(int(obj->updateText(Font.c_str(), FontSize, bBold, bItalic)));
	}
	else
		WRITE((int) 0);
}

void BoxCreate(Serializer& serializerIn, Serializer& serializerOut)
{
	READ(int, x); 
	READ(int, y); 
	READ(int, w); 
	READ(int, h); 
	READ(unsigned int, dwColor); 
	READ(bool, bShow);

	WRITE(g_pRenderer.add(std::make_shared<Box>(&g_pRenderer, x, y, w, h, dwColor, bShow)));
}

void BoxDestroy(Serializer& serializerIn, Serializer& serializerOut)
{
	READ(int, id);
	WRITE((int) g_pRenderer.remove(id));
}

void BoxSetShown(Serializer& serializerIn, Serializer& serializerOut)
{
	READ(int, id); 
	READ(bool, bShown);

	auto obj = g_pRenderer.getAs<Box>(id);
	if (obj)
	{
		obj->setShown(bShown);
		WRITE(int(1));
	}
	else
		WRITE(int(0));
}

void BoxSetBorder(Serializer& serializerIn, Serializer& serializerOut)
{
	READ(int, id); 
	READ(int, height); 
	READ(bool, bShown);

	auto obj = g_pRenderer.getAs<Box>(id);
	if (obj)
	{
		obj->setBorderWidth(height);
		obj->setBorderShown(bShown);
		WRITE(int(1));
	}
	else
		WRITE(int(0));
}

void BoxSetBorderColor(Serializer& serializerIn, Serializer& serializerOut)
{
	READ(int, id); 
	READ(unsigned int, dwColor);

	auto obj = g_pRenderer.getAs<Box>(id);
	if (obj)
	{
		obj->setBorderColor(dwColor);
		WRITE(int(1));
	}
	else
		WRITE(int(0));
}

void BoxSetColor(Serializer& serializerIn, Serializer& serializerOut)
{
	READ(int, id); 
	READ(unsigned int, dwColor);

	auto obj = g_pRenderer.getAs<Box>(id);
	if (obj)
	{
		obj->setBoxColor(dwColor);
		WRITE(int(1));
	}
	else
		WRITE(int(0));
}

void BoxSetHeight(Serializer& serializerIn, Serializer& serializerOut)
{
	READ(int, id); 
	READ(int, height);

	auto obj = g_pRenderer.getAs<Box>(id);
	if (obj)
	{
		obj->setBoxHeight(height);
		WRITE(int(1));
	}
	else
		WRITE(int(0));
}

void BoxSetPos(Serializer& serializerIn, Serializer& serializerOut)
{
	READ(int, id); 
	READ(int, x); 
	READ(int, y);

	auto obj = g_pRenderer.getAs<Box>(id);
	if (obj)
	{
		obj->setPos(x, y);
		WRITE(int(1));
	}
	else
		WRITE(int(0));
}

void BoxSetWidth(Serializer& serializerIn, Serializer& serializerOut)
{
	READ(int, id); 
	READ(int, width);

	auto obj = g_pRenderer.getAs<Box>(id);
	if (obj)
	{
		obj->setBoxWidth(width);
		WRITE(int(1));
	}
	else
		WRITE(int(0));
}

void LineCreate(Serializer& serializerIn, Serializer& serializerOut)
{
	READ(int, x1); 
	READ(int, y1); 
	READ(int, x2); 
	READ(int, y2); 
	READ(int, width); 
	READ(unsigned int, color);
	READ(bool, bShow);

	WRITE(g_pRenderer.add(std::make_shared<Line>(&g_pRenderer, x1, y1, x2, y2, width, color, bShow)));
}

void LineDestroy(Serializer& serializerIn, Serializer& serializerOut)
{
	READ(int, id);
	WRITE((int) g_pRenderer.remove(id));
}

void LineSetShown(Serializer& serializerIn, Serializer& serializerOut)
{
	READ(int, id); 
	READ(bool, bShown);

	auto obj = g_pRenderer.getAs<Line>(id);
	if (obj)
	{
		obj->setShown(bShown);
		WRITE(int(1));
	}
	else
		WRITE(int(0));
}

void LineSetColor(Serializer& serializerIn, Serializer& serializerOut)
{
	READ(int, id); 
	READ(unsigned int, color);

	auto obj = g_pRenderer.getAs<Line>(id);
	if (obj)
	{
		obj->setColor(color);
		WRITE(int(1));
	}
	else
		WRITE(int(0));
}

void LineSetWidth(Serializer& serializerIn, Serializer& serializerOut)
{
	READ(int, id); 
	READ(int, width);

	auto obj = g_pRenderer.getAs<Line>(id);
	if (obj)
	{
		obj->setWidth(width);
		WRITE(int(1));
	}
	else
		WRITE(int(0));
}

void LineSetPos(Serializer& serializerIn, Serializer& serializerOut)
{
	READ(int, id); 
	READ(int, x1); 
	READ(int, y1); 
	READ(int, x2); 
	READ(int, y2);

	auto obj = g_pRenderer.getAs<Line>(id);
	if (obj)
	{
		obj->setPos(x1, y1, x2, y2);
		WRITE(int(1));
	}
	else
		WRITE(int(0));
}

void ImageCreate(Serializer& serializerIn, Serializer& serializerOut)
{
	READ(std::string, path); 
	READ(int, x); 
	READ(int, y); 
	READ(int, rotation); 
	READ(int, align); 
	READ(bool, show);
	
	WRITE(g_pRenderer.add(std::make_shared<Image>(&g_pRenderer, path.c_str(), x, y, rotation, align, show)));
}

void ImageDestroy(Serializer& serializerIn, Serializer& serializerOut)
{
	READ(int, id);
	WRITE((int) g_pRenderer.remove(id));
}

void ImageSetShown(Serializer& serializerIn, Serializer& serializerOut)
{
	READ(int, id); 
	READ(bool, bShow);

	auto obj = g_pRenderer.getAs<Image>(id);
	if (obj)
	{
		obj->setShown(bShow);
		WRITE(int(1));
	}
	else
		WRITE(int(0));
}

void ImageSetAlign(Serializer& serializerIn, Serializer& serializerOut)
{
	READ(int, id); 
	READ(int, align);

	auto obj = g_pRenderer.getAs<Image>(id);
	if (obj)
	{
		obj->setAlign(align);
		WRITE(int(1));
	}
	else
		WRITE(int(0));
}

void ImageSetPos(Serializer& serializerIn, Serializer& serializerOut)
{
	READ(int, id); 
	READ(int, x); 
	READ(int, y);

	auto obj = g_pRenderer.getAs<Image>(id);
	if (obj)
	{
		obj->setPos(x, y);
		WRITE(int(1));
	}
	else
		WRITE(int(0));
}

void ImageSetRotation(Serializer& serializerIn, Serializer& serializerOut)
{
	READ(int, id); 
	READ(int, rotation);

	auto obj = g_pRenderer.getAs<Image>(id);
	if (obj)
	{
		obj->setRotation(rotation);
		WRITE(int(1));
	}
	else
		WRITE(int(0));
}


void DestroyAllVisual(Serializer& serializerIn, Serializer& serializerOut)
{
	g_pRenderer.destroyAll();
}

void ShowAllVisual(Serializer& serializerIn, Serializer& serializerOut)
{
	g_pRenderer.showAll();
}

void HideAllVisual(Serializer& serializerIn, Serializer& serializerOut)
{
	g_pRenderer.hideAll();
}

void GetFrameRate(Serializer& serializerIn, Serializer& serializerOut)
{
	WRITE(g_pRenderer.frameRate());
}

void GetScreenSpecs(Serializer& serializerIn, Serializer& serializerOut)
{
	WRITE(g_pRenderer.screenWidth()); 
	WRITE(g_pRenderer.screenHeight());
}

void SetCalculationRatio(Serializer& serializerIn, Serializer& serializerOut)
{
	READ(int, width);
	READ(int, height);

	RenderBase::xCalculator = width;
	RenderBase::yCalculator = height;
}

void SetOverlayPriority(Serializer& serializerIn, Serializer& serializerOut)
{
	READ(int, id);
	READ(int, priority);

	auto obj = g_pRenderer.get(id);
	if (obj)
	{
		obj->setPriority(priority);
		WRITE(int(1));
	}
	else
		WRITE(int(0));
}

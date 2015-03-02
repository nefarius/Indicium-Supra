#include <Utils/SafeBlock.h>

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

	WRITE(g_pRenderer.add(std::make_shared<Text>(&g_pRenderer, Font, FontSize, bBold, bItalic, x, y, color, string, bShadow, bShow)));
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
	
	WRITE(int(safeExecuteWithValidation([&](){ 
		g_pRenderer.getAs<Text>(id)->setShadow(bShadow); 
	})));
}

void TextSetShown(Serializer& serializerIn, Serializer& serializerOut)
{
	READ(int, id); 
	READ(bool, bShown);

	WRITE(int(safeExecuteWithValidation([&](){
		g_pRenderer.getAs<Text>(id)->setShown(bShown);
	})));
}

void TextSetColor(Serializer& serializerIn, Serializer& serializerOut)
{
	READ(int, id); 
	READ(unsigned int, color);

	WRITE(int(safeExecuteWithValidation([&](){
		g_pRenderer.getAs<Text>(id)->setColor(color);
	})));
}

void TextSetPos(Serializer& serializerIn, Serializer& serializerOut)
{
	READ(int, id); 
	READ(int, x); 
	READ(int, y);

	WRITE(int(safeExecuteWithValidation([&](){
		g_pRenderer.getAs<Text>(id)->setPos(x, y);
	})));
}

void TextSetString(Serializer& serializerIn, Serializer& serializerOut)
{
	READ(int, id); 
	READ(std::string, str);

	WRITE(int(safeExecuteWithValidation([&](){
		g_pRenderer.getAs<Text>(id)->setText(str);
	})));
}

void TextUpdate(Serializer& serializerIn, Serializer& serializerOut)
{
	READ(int, id); 
	READ(std::string, Font); 
	READ(int, FontSize); 
	READ(bool, bBold); 
	READ(bool, bItalic);

	WRITE(int(safeExecuteWithValidation([&](){
		g_pRenderer.getAs<Text>(id)->updateText(Font, FontSize, bBold, bItalic);
	})));
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

	WRITE(int(safeExecuteWithValidation([&](){
		g_pRenderer.getAs<Box>(id)->setShown(bShown);
	})));
}

void BoxSetBorder(Serializer& serializerIn, Serializer& serializerOut)
{
	READ(int, id); 
	READ(int, height); 
	READ(bool, bShown);

	WRITE(int(safeExecuteWithValidation([&](){
		g_pRenderer.getAs<Box>(id)->setBorderWidth(height);
		g_pRenderer.getAs<Box>(id)->setBorderShown(bShown);
	})));
}

void BoxSetBorderColor(Serializer& serializerIn, Serializer& serializerOut)
{
	READ(int, id); 
	READ(unsigned int, dwColor);

	WRITE(int(safeExecuteWithValidation([&](){
		g_pRenderer.getAs<Box>(id)->setBorderColor(dwColor);
	})));
}

void BoxSetColor(Serializer& serializerIn, Serializer& serializerOut)
{
	READ(int, id); 
	READ(unsigned int, dwColor);

	WRITE(int(safeExecuteWithValidation([&](){
		g_pRenderer.getAs<Box>(id)->setBoxColor(dwColor);
	})));
}

void BoxSetHeight(Serializer& serializerIn, Serializer& serializerOut)
{
	READ(int, id); 
	READ(int, height);

	WRITE(int(safeExecuteWithValidation([&](){
		g_pRenderer.getAs<Box>(id)->setBoxHeight(height);
	})));
}

void BoxSetPos(Serializer& serializerIn, Serializer& serializerOut)
{
	READ(int, id); 
	READ(int, x); 
	READ(int, y);

	WRITE(int(safeExecuteWithValidation([&](){
		g_pRenderer.getAs<Box>(id)->setPos(x, y);
	})));
}

void BoxSetWidth(Serializer& serializerIn, Serializer& serializerOut)
{
	READ(int, id); 
	READ(int, width);

	WRITE(int(safeExecuteWithValidation([&](){
		g_pRenderer.getAs<Box>(id)->setBoxWidth(width);
	})));
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

	WRITE(int(safeExecuteWithValidation([&](){
		g_pRenderer.getAs<Line>(id)->setShown(bShown);
	})));
}

void LineSetColor(Serializer& serializerIn, Serializer& serializerOut)
{
	READ(int, id); 
	READ(unsigned int, color);

	WRITE(int(safeExecuteWithValidation([&](){
		g_pRenderer.getAs<Line>(id)->setColor(color);
	})));
}

void LineSetWidth(Serializer& serializerIn, Serializer& serializerOut)
{
	READ(int, id); 
	READ(int, width);

	WRITE(int(safeExecuteWithValidation([&](){
		g_pRenderer.getAs<Line>(id)->setWidth(width);
	})));
}

void LineSetPos(Serializer& serializerIn, Serializer& serializerOut)
{
	READ(int, id); 
	READ(int, x1); 
	READ(int, y1); 
	READ(int, x2); 
	READ(int, y2);

	WRITE(int(safeExecuteWithValidation([&](){
		g_pRenderer.getAs<Line>(id)->setPos(x1, y1, x2, y2);
	})));
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

	WRITE(int(safeExecuteWithValidation([&](){
		g_pRenderer.getAs<Image>(id)->setShown(bShow);
	})));
}

void ImageSetAlign(Serializer& serializerIn, Serializer& serializerOut)
{
	READ(int, id); 
	READ(int, align);

	WRITE(int(safeExecuteWithValidation([&](){
		g_pRenderer.getAs<Image>(id)->setAlign(align);
	})));
}

void ImageSetPos(Serializer& serializerIn, Serializer& serializerOut)
{
	READ(int, id); 
	READ(int, x); 
	READ(int, y);

	WRITE(int(safeExecuteWithValidation([&](){
		g_pRenderer.getAs<Image>(id)->setPos(x, y);
	})));
}

void ImageSetRotation(Serializer& serializerIn, Serializer& serializerOut)
{
	READ(int, id); 
	READ(int, rotation);

	WRITE(int(safeExecuteWithValidation([&](){
		g_pRenderer.getAs<Image>(id)->setRotation(rotation);
	})));
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

	WRITE(int(safeExecuteWithValidation([&](){
		g_pRenderer.get(id)->setPriority(priority);
	})));
}
